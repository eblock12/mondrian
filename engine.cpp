#include "engine.h"
#include "application.h"

Engine::Engine()
    : levelFileName(0), level(0), renderer(0), camera(0), timeInterval(0.0f)
{
    camera = new Camera();
    renderer = new Renderer();
    if (renderer->Initialize(Application::GetSingleton().GetWindowHandle()) == false)
    {
        PostQuitMessage(0);
        return;
    }
    renderer->SetCurrentCamera(camera);

    AddObject(new GameHandler());
}

Engine::~Engine()
{
    for (std::list<Object *>::iterator i = objectList.begin(); i != objectList.end(); i++)
         *i;
    for (std::vector<SpriteMap *>::iterator i = spriteMaps.begin(); i != spriteMaps.end(); i++)
        delete *i;
    if (camera)
        delete camera;
    if (level)
        delete level;
    if (renderer)
        delete renderer;
}

void Engine::SetLevelFileName(char *fileName)
{
    if (levelFileName)
        delete levelFileName;
    levelFileName = new char[strlen(fileName) + 1];
    strcpy(levelFileName, fileName);
}

void Engine::Tick()
{
    while (timeInterval >= TIME_DELTA)
    {
        timeInterval -= TIME_DELTA;
        camera->Tick(TIME_DELTA);
        level->Tick(TIME_DELTA);
    
        while (expireObjectList.empty() == false)
        {
            for (std::list<Object *>::iterator i = expireObjectList.begin(); i != expireObjectList.end(); i++)
            {
                delete (*i);
                DeleteObject(*i);
                expireObjectList.erase(i);
                break;
            }
        }

        for (std::list<Object *>::iterator i = objectList.begin(); i != objectList.end(); i++)
            (*i)->Tick(TIME_DELTA);
    }
    renderer->Render();
}

bool Engine::LoadLevel()
{
    if (levelFileName == 0)
        return false;

    Application *app = Application::GetSingletonPtr();

    FILE *file = fopen(levelFileName, "rbS");
    if (file == 0)
    {
        app->ErrorMsg("Failed to the load level file '%s', file was not found.", levelFileName);
        return false;
    }

    unsigned char reserved[64];

    char fileId[4];
    fread(&fileId, sizeof(char), 3, file);
    fileId[3] = 0;
    if (strcmp(fileId, "BML"))
    {
        app->ErrorMsg("Failed to the load level file '%s', file was invalid.", levelFileName);
        fclose(file);
        return false;
    }

    unsigned char fileVersion;
    fread(&fileVersion, sizeof(char), 1, file);
    if (fileVersion > BML_VERSION)
    {
        app->ErrorMsg("The version of this file is unsupported by this build of the game.");
        fclose(file);
        return false;
    }

    unsigned int numTilesets;
    fread(&numTilesets, sizeof(unsigned int), 1, file);

    unsigned int layerCount;
    fread(&layerCount, sizeof(unsigned int), 1, file);

    unsigned char bgRed, bgGreen, bgBlue;
    fread(&bgRed, sizeof(unsigned char), 1, file);
    fread(&bgGreen, sizeof(unsigned char), 1, file);
    fread(&bgBlue, sizeof(unsigned char), 1, file);

    fread(&reserved, sizeof(unsigned char), 64, file);

    if (level)
        delete level;

    level = new Level();
    renderer->SetCurrentLevel(level);
    level->SetBackgroundColor(bgRed, bgGreen, bgBlue);

    char *filePath = new char[(int)strlen(levelFileName) + 1];
    strcpy(filePath, levelFileName);
    int i = (int)strlen(levelFileName);
    while (filePath[i] != '\\') i--;
    filePath[i+1] = 0;

    for (unsigned int i = 0; i < numTilesets; i++)
    {
        unsigned int fileNameLength;
        fread(&fileNameLength, sizeof(unsigned int), 1, file);
        char *tilesetFileName = new char[fileNameLength + 1];
        fread(tilesetFileName, sizeof(char), fileNameLength, file);
        tilesetFileName[fileNameLength] = 0;
        char *fullTilesetPath = new char[strlen(filePath) + fileNameLength + 2];
        sprintf(fullTilesetPath, "%s%s", filePath, tilesetFileName);

        FILE *tilesetFile = fopen(fullTilesetPath, "rb");
        if (tilesetFile == 0)
        {
            app->ErrorMsg("Failed to load tileset file '%s'\nThe tilesets used by this level must be in the same directory as the level file itself.", fullTilesetPath);
            fclose(file);
            return false;
        }
        fclose(tilesetFile);

        if (LoadTileset(fullTilesetPath) == false)
            return false;

        delete fullTilesetPath;
        delete tilesetFileName;

        fread(&reserved, sizeof(unsigned char), 8, file);
    }

    for (unsigned int i = 0; i < layerCount; i++)
    {
        unsigned int layerId;
        fread(&layerId, sizeof(unsigned int), 1, file);
        level->AddLayerSortId(layerId);
    }

    for (unsigned int i = 0; i < layerCount; i++)
    {
        Layer *layer = new Layer;
        unsigned int nameLength;
        fread(&nameLength, sizeof(unsigned int), 1, file);
        layer->name = new char[nameLength + 1];
        layer->name[nameLength] = 0;
        fread(layer->name, sizeof(char), nameLength, file);
        fread(&layer->width, sizeof(unsigned int), 1, file);
        fread(&layer->height, sizeof(unsigned int), 1, file);
        fread(&layer->enableParallax, sizeof(bool), 1, file);
        fread(&layer->parallaxSpeed, sizeof(int), 1, file);
        fread(&layer->enableAutoScroll, sizeof(bool), 1, file);
        fread(&layer->autoScrollXSpeed, sizeof(float), 1, file);
        fread(&layer->autoScrollYSpeed, sizeof(float), 1, file);
        layer->autoScrollXSpeed /= TIME_DELTA;
        layer->autoScrollYSpeed /= TIME_DELTA;
        fread(&layer->enableMirrorX, sizeof(bool), 1, file);
        fread(&layer->enableMirrorY, sizeof(bool), 1, file);
        fread(&layer->globalAlpha, sizeof(unsigned char), 1, file);
        layer->isVisible = true;
        layer->autoScrollX = layer->autoScrollY = 0.0f;
        fread(&reserved, sizeof(unsigned char), 8, file);
        layer->tiles = new Tile[layer->width * layer->height];
        fread(layer->tiles, sizeof(Tile), layer->width * layer->height, file);
        level->AddLayer(layer);
    }

    fclose(file);
    return true;
}

bool Engine::LoadTileset(char *fileName)
{
    Application *app = Application::GetSingletonPtr();

    char *extension = fileName + strlen(fileName);
    while (*extension != '.') extension--;
    extension++;

    if (!stricmp(extension, "tga"))
    {
        TGA::TGAImage tgaImage;
        FILE *file = fopen(fileName, "rb");
        if (!file)
        {
            app->ErrorMsg("Failed to open '%s'", fileName);
            return false;
        }
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fclose(file);
        file = fopen(fileName, "rb");
        tgaImage.LoadFromStream(file, fileSize);
        fclose(file);
        Color *surface = new Color[tgaImage.header.imageInfo.width * tgaImage.header.imageInfo.height];
        tgaImage.DrawToBitmap(surface);
        unsigned int textureId = renderer->LoadTexture(tgaImage.header.imageInfo.width, tgaImage.header.imageInfo.height, surface);
        delete[] surface;
        Tileset *tileset = new Tileset(tgaImage.header.imageInfo.width, tgaImage.header.imageInfo.height, textureId);
        level->AddTileset(tileset);
    }
    else
    {
        app->ErrorMsg("BMP tilesets are not supported.");
        return false;
    }
    return true;
}

bool Engine::LoadSpriteMap(char *fileName)
{
    Application *app = Application::GetSingletonPtr();

    char *extension = fileName + strlen(fileName);
    while (*extension != '.') extension--;
    extension++;

    if (!stricmp(extension, "tga"))
    {
        TGA::TGAImage tgaImage;
        FILE *file = fopen(fileName, "rb");
        if (!file)
        {
            app->ErrorMsg("Failed to open '%s'", fileName);
            return false;
        }
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fclose(file);
        file = fopen(fileName, "rb");
        tgaImage.LoadFromStream(file, fileSize);
        fclose(file);
        Color *surface = new Color[tgaImage.header.imageInfo.width * tgaImage.header.imageInfo.height];
        tgaImage.DrawToBitmap(surface);
        unsigned int textureId = renderer->LoadTexture(tgaImage.header.imageInfo.width, tgaImage.header.imageInfo.height, surface);
        delete[] surface;
        SpriteMap *spriteMap = new SpriteMap(tgaImage.header.imageInfo.width, tgaImage.header.imageInfo.height, textureId);
        AddSpriteMap(spriteMap);
    }
    else
    {
        app->ErrorMsg("BMP sprite maps are not supported.");
        return false;
    }
    return true;
}

void Engine::DeleteObject(Object *object)
{
    for (std::list<Object *>::iterator i = objectList.begin(); i != objectList.end(); i++)
    {
        if (object == (*i))
        {
            objectList.erase(i);
            return;
        }
    }
}

bool Engine::isColliding(int x, int y)
{
    if (x < 0 || y < 0)
        return true;

    int xTile = x / TILE_SIZE;
    int yTile = y / TILE_SIZE;

    if (xTile >= (int)level->GetWidth() || yTile >= (int)level->GetHeight())
        return true;

    if (level->GetTile(x / TILE_SIZE, y / TILE_SIZE)->tileId == 0)
        return false;
    else
        return true;
}