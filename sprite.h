#pragma once

enum 
{
    SPRITE_NONE            = 0x00,
    SPRITE_FLIP_HORIZONTAL = 0x01,
    SPRITE_FLIP_VERTICAL   = 0x02
};

class Sprite
{
public:
    Sprite(int newSpriteMapId, int newMapCoordId, unsigned int newWidth, unsigned int newHeight);
    ~Sprite();
    int GetAttributes() { return attributes; }
    int GetPositionX() { return positionX; }
    int GetPositionY() { return positionY; }
    unsigned int GetWidth() { return width; }
    unsigned int GetHeight() { return height; }
    int GetSpriteMapId() { return spriteMapId; }
    int GetSpriteMapCoordId() { return mapCoordId; }
    void SetPosition(int newX, int newY) { positionX = newX; positionY = newY; }
    void SetAttributes(int newAttributes) { attributes = newAttributes; }
private:
    int positionX;
    int positionY;
    unsigned int width;
    unsigned int height;
    int spriteMapId;
    int mapCoordId;
    int attributes;
};