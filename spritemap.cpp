#include "spritemap.h"
#include "renderer.h"

SpriteMap::SpriteMap(unsigned int width, unsigned int height, unsigned int textureId)
{
    this->textureId = textureId;
    this->width = width;
    this->height = height;
}

SpriteMap::~SpriteMap()
{
    Renderer::GetSingleton().DeleteTexture(textureId);
}

void SpriteMap::AddTextureCoords(int x1, int y1, int x2, int y2)
{
    TextureCoords *tc = new TextureCoords;
    tc->x1 = (float)x1 / (float)width;
    tc->y1 = (float)y1 / (float)height;
    tc->x2 = (float)x2 / (float)width;
    tc->y2 = (float)y2 / (float)height;
    AddTextureCoords(tc);
}