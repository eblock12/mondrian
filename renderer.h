#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <vector>

#include "singleton.h"
#include "level.h"
#include "camera.h"
#include "sprite.h"

using namespace std;

const int MAX_TEXTURES = 8;

class Renderer : public Singleton<Renderer>
{
public:
    Renderer();
    ~Renderer();
    bool Initialize(HWND window);
    void AddSprite(Sprite *sprite) { sprites.push_back(sprite); }
    void DeleteSprite(Sprite *sprite);
    void Render();
    void SetCurrentLevel(Level *newLevel) { level = newLevel; }
    void SetCurrentCamera(Camera *newCamera) { camera = newCamera; }
    void SetViewportSize(int newWidth, int newHeight);
    unsigned int LoadTexture(unsigned int width, unsigned int height, void *data);
    void DeleteTexture(unsigned int textureId);
    bool inViewport(int x, int y);
private:
    Camera *camera;
    Level *level;
    vector<Sprite *> sprites;
    HWND windowHandle;
    HDC dcHandle;
    HGLRC rcHandle;
    unsigned int textureIds[MAX_TEXTURES];
    int textureIdx;
};