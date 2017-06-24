#pragma once

#include <vector>
#include <list>

#include "renderer.h"
#include "level.h"
#include "tga.h"
#include "tileset.h"
#include "camera.h"
#include "object.h"
#include "singleton.h"
#include "spritemap.h"
#include "game_select.h"

#ifdef USE_BLASTER_MASTER
#include "bm-game_handler.h"
using namespace BlasterMaster;
#else
#endif

const int BML_VERSION = 1;
const float TIME_DELTA = 0.01f;

using namespace std;

class Engine : public Singleton<Engine>
{
public:
    Engine();
    ~Engine();
    void Tick();
    void AddObject(Object *newObject) { objectList.push_back(newObject); }
    void AddExpiredObject(Object *object) { expireObjectList.push_back(object); }
    void AddSpriteMap(SpriteMap *spriteMap) { spriteMaps.push_back(spriteMap); }
    bool LoadLevel();
    bool LoadTileset(char *fileName);
    bool LoadSpriteMap(char *fileName);
    void SetLevelFileName(char *fileName);
    void RequestViewportResize(int newWidth, int newHeight) { renderer->SetViewportSize(newWidth, newHeight); }
    void DeleteObject(Object *object);
    bool isColliding(int x, int y);
    SpriteMap *GetSpriteMap(int id) { return spriteMaps[id]; }
    Camera *GetCamera() { return camera; }
    float timeInterval;
private:
    char *levelFileName;
    Renderer *renderer;
    Level *level;
    Camera *camera;
    vector<SpriteMap *> spriteMaps;
    list<Object *> objectList;
    list<Object *> expireObjectList;
};