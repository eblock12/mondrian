#include "renderer.h"
#include "application.h"

Renderer::Renderer()
    : level(0), textureIdx(0)
{
}

Renderer::~Renderer()
{
    for (std::vector<Sprite *>::iterator i = sprites.begin(); i != sprites.end(); i++)
        delete *i;

    if (rcHandle)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(rcHandle);
    }
    if (dcHandle)
        ReleaseDC(windowHandle, dcHandle);
}

bool Renderer::Initialize(HWND window)
{
    Application *app = Application::GetSingletonPtr();

    PIXELFORMATDESCRIPTOR pfd;
    int pixelFormat;

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.dwLayerMask = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 0;
    pfd.cStencilBits = 0;

    windowHandle = window;
    dcHandle = GetDC(window);

    pixelFormat = ChoosePixelFormat(dcHandle, &pfd);
    if (!pixelFormat)
    {
        pfd.cColorBits = pfd.cDepthBits = 16;
        pixelFormat = ChoosePixelFormat(dcHandle, &pfd);
        if (!pixelFormat)
        {
            app->ErrorMsg("Unable to choose pixel format. 16 or 32-bit color depth support is required");
            return false;
        }
    }

    if (!SetPixelFormat(dcHandle, pixelFormat, &pfd))
    {
        app->ErrorMsg("Unable to set pixel format");
        return false;
    }

    if (!(rcHandle = wglCreateContext(dcHandle)))
    {
        app->ErrorMsg("Unable to create OpenGL rendering context");
        return false;
    }

    if (!wglMakeCurrent(dcHandle, rcHandle))
    {
        app->ErrorMsg("Unable to set OpenGL rendering context");
        return false;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    RECT r;
    GetClientRect(windowHandle, &r);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, r.right, r.bottom);
    gluOrtho2D(0.0, (double)r.right, 0.0, (double)r.bottom);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGenTextures(MAX_TEXTURES, textureIds);

    return true;    
}

void Renderer::SetViewportSize(int newWidth, int newHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, newWidth, newHeight);
    gluOrtho2D(0.0, (double)newWidth, 0.0, (double)newHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::Render()
{
    if (level == 0)
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    {
        glClearColor(level->GetBackgroundColorR(), level->GetBackgroundColorG(), level->GetBackgroundColorB(), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        RECT r;
        GetClientRect(windowHandle, &r);

        unsigned int width = level->GetWidth();
        unsigned int height = level->GetHeight();

        unsigned int lastTextureId = 0;

        glBegin(GL_QUADS);
        for (unsigned int layerOrderId = 0; layerOrderId < level->GetLayerCount(); layerOrderId++)
        {
            unsigned int layerId = level->GetLayerOrderId(layerOrderId);
            Layer *layer = level->GetLayer(layerId);
            int scrollX, scrollY;
            scrollX = int((float)camera->GetPositionX() * (layer->enableParallax == true ? (float)layer->parallaxSpeed / 100.0f : 1.0f)) + (int)layer->autoScrollX;
            scrollY = int((float)camera->GetPositionY() * (layer->enableParallax == true ? (float)layer->parallaxSpeed / 100.0f : 1.0f)) + (int)layer->autoScrollY;
            glColor4ub(255, 255, 255, layer->globalAlpha);

            int maxY = scrollY / TILE_SIZE + (r.bottom / TILE_SIZE) + 2;
            int maxX = scrollX / TILE_SIZE + (r.right / TILE_SIZE) + 2;

            for (int yTile = scrollY / TILE_SIZE; yTile < maxY; yTile++)
            {
                for (int xTile = scrollX / TILE_SIZE; xTile < maxX; xTile++)
                {
                    int x = xTile * TILE_SIZE - scrollX;
                    int y = r.bottom - (yTile * TILE_SIZE - scrollY);
                    Tile *tile = level->GetTile(layerId, xTile, yTile);
                    if (tile != 0)
                    {
                        if (tile->tileId != 0)
                        {
                            Tileset *tileset = level->GetTileset(tile->tileSetId);
                            float tileSetWidth = (float)tileset->GetWidth();
                            float tileSetHeight = (float)tileset->GetHeight();
                            RECT tileRect;
                            tileset->GetTileRect(tile->tileId, &tileRect);
                            if (lastTextureId != tileset->GetTextureId())
                            {
                                lastTextureId = tileset->GetTextureId();
                                glEnd();
                                glBindTexture(GL_TEXTURE_2D, tileset->GetTextureId());
                                glBegin(GL_QUADS);
                            }
                            if ((tile->tileAttributes & TILE_FLIP_HORIZONTAL) == TILE_FLIP_HORIZONTAL && (tile->tileAttributes & TILE_FLIP_VERTICAL) == TILE_FLIP_VERTICAL)
                            {
                                glTexCoord2f((float)tileRect.right / tileSetWidth, (float)tileRect.top / tileSetHeight);
                                glVertex2i(x, y - TILE_SIZE);
                                glTexCoord2f((float)tileRect.left / tileSetWidth, (float)tileRect.top / tileSetHeight);
                                glVertex2i(x + TILE_SIZE, y - TILE_SIZE);
                                glTexCoord2f((float)tileRect.left / tileSetWidth, (float)tileRect.bottom / tileSetHeight);
                                glVertex2i(x + TILE_SIZE, y);
                                glTexCoord2f((float)tileRect.right / tileSetWidth, (float)tileRect.bottom / tileSetHeight);
                                glVertex2i(x, y);
                            }
                            else if ((tile->tileAttributes & TILE_FLIP_HORIZONTAL) == TILE_FLIP_HORIZONTAL)
                            {
                                glTexCoord2f((float)tileRect.right / tileSetWidth, (float)tileRect.bottom / tileSetHeight);
                                glVertex2i(x, y - TILE_SIZE);
                                glTexCoord2f((float)tileRect.left / tileSetWidth, (float)tileRect.bottom / tileSetHeight);
                                glVertex2i(x + TILE_SIZE, y - TILE_SIZE);
                                glTexCoord2f((float)tileRect.left / tileSetWidth, (float)tileRect.top / tileSetHeight);
                                glVertex2i(x + TILE_SIZE, y);
                                glTexCoord2f((float)tileRect.right / tileSetWidth, (float)tileRect.top / tileSetHeight);
                                glVertex2i(x, y);
                            }
                            else if ((tile->tileAttributes & TILE_FLIP_VERTICAL) == TILE_FLIP_VERTICAL)
                            {
                                glTexCoord2f((float)tileRect.left / tileSetWidth, (float)tileRect.top / tileSetHeight);
                                glVertex2i(x, y - TILE_SIZE);
                                glTexCoord2f((float)tileRect.right / tileSetWidth, (float)tileRect.top / tileSetHeight);
                                glVertex2i(x + TILE_SIZE, y - TILE_SIZE);
                                glTexCoord2f((float)tileRect.right / tileSetWidth, (float)tileRect.bottom / tileSetHeight);
                                glVertex2i(x + TILE_SIZE, y);
                                glTexCoord2f((float)tileRect.left / tileSetWidth, (float)tileRect.bottom / tileSetHeight);
                                glVertex2i(x, y);
                            }
                            else
                            {
                                glTexCoord2f((float)tileRect.left / tileSetWidth, (float)tileRect.bottom / tileSetHeight);
                                glVertex2i(x, y - TILE_SIZE);
                                glTexCoord2f((float)tileRect.right / tileSetWidth, (float)tileRect.bottom / tileSetHeight);
                                glVertex2i(x + TILE_SIZE, y - TILE_SIZE);
                                glTexCoord2f((float)tileRect.right / tileSetWidth, (float)tileRect.top / tileSetHeight);
                                glVertex2i(x + TILE_SIZE, y);
                                glTexCoord2f((float)tileRect.left / tileSetWidth, (float)tileRect.top / tileSetHeight);
                                glVertex2i(x, y);
                            }
                        }
                    }
                }
            }
            if (level->GetBaseLayer() == layerId)
            {
                for (std::vector<Sprite *>::iterator i = sprites.begin(); i != sprites.end(); i++)
                {
                    Sprite *sprite = *i;
                    int x = sprite->GetPositionX() - (int)camera->GetPositionX();
                    int y = (r.bottom - sprite->GetPositionY() + (int)camera->GetPositionY());
                    int width = sprite->GetWidth();
                    int height = sprite->GetHeight();
                    SpriteMap *sMap = Engine::GetSingleton().GetSpriteMap(sprite->GetSpriteMapId());
                    TextureCoords *tc = sMap->GetTextureCoords(sprite->GetSpriteMapCoordId());

                    if (lastTextureId != sMap->GetTextureId())
                    {
                        lastTextureId = sMap->GetTextureId();
                        glEnd();
                        glBindTexture(GL_TEXTURE_2D, sMap->GetTextureId());
                        glBegin(GL_QUADS);
                    }

                    int attr = sprite->GetAttributes();

                    if ((attr & SPRITE_FLIP_HORIZONTAL) == SPRITE_FLIP_HORIZONTAL && (attr & SPRITE_FLIP_VERTICAL) == SPRITE_FLIP_VERTICAL)
                    {
                        glTexCoord2f(tc->x1, tc->y2);
                        glVertex2i(x, y - height);
                        glTexCoord2f(tc->x2, tc->y2);
                        glVertex2i(x + width, y - height);
                        glTexCoord2f(tc->x2, tc->y1);
                        glVertex2i(x + width, y);
                        glTexCoord2f(tc->x1, tc->y1);
                        glVertex2i(x, y);
                    }
                    else if ((attr & SPRITE_FLIP_HORIZONTAL) == SPRITE_FLIP_HORIZONTAL)
                    {
                        glTexCoord2f(tc->x2, tc->y2);
                        glVertex2i(x, y - height);
                        glTexCoord2f(tc->x1, tc->y2);
                        glVertex2i(x + width, y - height);
                        glTexCoord2f(tc->x1, tc->y1);
                        glVertex2i(x + width, y);
                        glTexCoord2f(tc->x2, tc->y1);
                        glVertex2i(x, y);
                    }
                    else if ((attr & SPRITE_FLIP_VERTICAL) == SPRITE_FLIP_VERTICAL)
                    {
                        glTexCoord2f(tc->x1, tc->y2);
                        glVertex2i(x, y - height);
                        glTexCoord2f(tc->x2, tc->y2);
                        glVertex2i(x + width, y - height);
                        glTexCoord2f(tc->x2, tc->y1);
                        glVertex2i(x + width, y);
                        glTexCoord2f(tc->x1, tc->y1);
                        glVertex2i(x, y);
                    }
                    else
                    {
                        glTexCoord2f(tc->x1, tc->y2);
                        glVertex2i(x, y - height);
                        glTexCoord2f(tc->x2, tc->y2);
                        glVertex2i(x + width, y - height);
                        glTexCoord2f(tc->x2, tc->y1);
                        glVertex2i(x + width, y);
                        glTexCoord2f(tc->x1, tc->y1);
                        glVertex2i(x, y);
                    }
                }
            }
        }

        glEnd();
    }
    SwapBuffers(dcHandle);
}

unsigned int Renderer::LoadTexture(unsigned int width, unsigned int height, void *data)
{
    unsigned int textureId;

    //glGenTextures(1, &textureId);
    textureId = textureIds[textureIdx];
    textureIdx++;
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureId;
}

void Renderer::DeleteTexture(unsigned int textureId)
{
    glDeleteTextures(1, &textureId);
}

void Renderer::DeleteSprite(Sprite *sprite)
{
    for (std::vector<Sprite *>::iterator i = sprites.begin(); i != sprites.end(); i++)
    {
        if (sprite == (*i))
        {
            sprites.erase(i);
            return;
        }
    }
}

bool Renderer::inViewport(int x, int y)
{
    int viewportWidth = Application::GetSingleton().GetWidth();
    int viewportHeight = Application::GetSingleton().GetHeight();
    int viewportX = x - (int)camera->GetPositionX();
    int viewportY = y - (int)camera->GetPositionY();

    if (viewportX < 0 || viewportY < 0 || viewportX > viewportWidth || viewportY > viewportHeight)
        return false;
    else
        return true;
}