#pragma once

#include <vector>

using namespace std;

struct TextureCoords
{
    float x1;
    float x2;
    float y1;
    float y2;
};

class SpriteMap
{
public:
    SpriteMap(unsigned int width, unsigned int height, unsigned int textureId);
    ~SpriteMap();
    void AddTextureCoords(int x1, int y1, int x2, int y2);
    void AddTextureCoords(TextureCoords *tc) { texCoords.push_back(tc); }
    TextureCoords *GetTextureCoords(int id) { return texCoords[id]; }
    unsigned int GetTextureId() { return textureId; }
private:
    unsigned int width;
    unsigned int height;
    unsigned int textureId;
    vector<TextureCoords *> texCoords;
};