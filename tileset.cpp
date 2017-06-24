#include "tileset.h"
#include "renderer.h"

Tileset::Tileset(unsigned int width, unsigned int height, unsigned int textureId)
{
    this->textureId = textureId;
    this->width = width;
    this->height = height;
    numTiles = (width / TILE_SIZE) * (height / TILE_SIZE);
}

Tileset::~Tileset()
{
    Renderer::GetSingleton().DeleteTexture(textureId);
}

void Tileset::GetTileRect(int tileNum, RECT *r)
{
    tileNum--;
    int yTile = tileNum / (width / TILE_SIZE);
    int xTile = tileNum - (yTile * (width / TILE_SIZE));
    r->left = xTile * TILE_SIZE;
    r->top = yTile * TILE_SIZE;
    r->right = r->left + TILE_SIZE;
    r->bottom = r->top + TILE_SIZE;
}