#pragma once

#include <vector>
using namespace std;

#include "tileset.h"

const int BASE_LAYER = 0;

struct Tile
{
    int tileSetId;
    int tileId;
    int tileAttributes;
};

struct Layer
{
    Tile *tiles;
    char *name;
    unsigned char globalAlpha;
    unsigned int width;
    unsigned int height;
    bool isVisible;
    bool enableMirrorX;
    bool enableMirrorY;
    bool enableParallax;
    bool enableAutoScroll;
    int parallaxSpeed;
    float autoScrollXSpeed;
    float autoScrollYSpeed;
    float autoScrollX;
    float autoScrollY;
};

class Level
{
public:
    Level();
    ~Level();
    void AddLayer(Layer *newLayer) { if (!strcmp(newLayer->name, "Base")) baseLayer = (int)layers.size(); layers.push_back(newLayer); }
    void AddLayerSortId(unsigned int layerId) { layerOrder.push_back(layerId); }
    void AddTileset(Tileset *tileset) { tilesets.push_back(tileset); }
    void SetBackgroundColor(unsigned char r, unsigned char g, unsigned char b) { bgRed = (float)r / 255.0f; bgGreen = (float)g / 255.0f; bgBlue = (float)b / 255.0f; }
    Layer *GetLayer(unsigned int id) { return layers[id]; }
    unsigned int GetWidth() { return layers[baseLayer]->width; }
    unsigned int GetHeight() { return layers[baseLayer]->height; }
    unsigned int GetLayerCount() { return (unsigned int)layers.size(); }
    unsigned int GetLayerOrderId(unsigned int i) { return layerOrder[i]; }
    Tile *GetTile(int layerId, int xTile, int yTile);
    Tile *GetTile(int xTile, int yTile) { return GetTile(baseLayer, xTile, yTile); }
    Tileset *GetTileset(int tileSetId) { return tilesets[tileSetId]; }
    float GetBackgroundColorR() { return bgRed; }
    float GetBackgroundColorG() { return bgGreen; }
    float GetBackgroundColorB() { return bgBlue; }
    void Tick(float timeDelta);
    int GetBaseLayer() { return baseLayer; }
private:
    float bgRed;
    float bgGreen;
    float bgBlue;
    vector<Layer *>layers;
    vector<unsigned int> layerOrder;
    vector<Tileset *> tilesets;
    int baseLayer;
};