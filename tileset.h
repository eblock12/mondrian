#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "color.h"

const int TILE_SIZE = 32;

enum 
{
    TILE_NONE            = 0x00,
    TILE_FLIP_HORIZONTAL = 0x01,
    TILE_FLIP_VERTICAL   = 0x02
};

class Tileset
{
public:
    Tileset(unsigned int width, unsigned int height, unsigned int textureId);
    ~Tileset();
    void GetTileRect(int tileNum, RECT *r);
    unsigned int GetTextureId() { return textureId; }
    int GetTileCount() { return numTiles; }
    unsigned int GetWidth() { return width; }
    unsigned int GetHeight() { return height; }
private:
    unsigned int textureId;
    unsigned int numTiles;
    unsigned int width;
    unsigned int height;
};