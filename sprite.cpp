#include "sprite.h"

Sprite::Sprite(int newSpriteMapId, int newMapCoordId, unsigned int newWidth, unsigned int newHeight)
{
    positionX = positionY = 0;
    spriteMapId = newSpriteMapId;
    mapCoordId = newMapCoordId;
    width = newWidth;
    height = newHeight;
    attributes = SPRITE_NONE;
}

Sprite::~Sprite()
{
}