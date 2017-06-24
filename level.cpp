#include "level.h"

Level::Level()
{
}

Level::~Level()
{
    for (std::vector<Tileset *>::iterator i = tilesets.begin(); i != tilesets.end(); i++)
        delete *i;
    tilesets.clear();

    for (vector<Layer *>::iterator i = layers.begin(); i != layers.end(); i++)
    {
        delete (*i)->tiles;
        delete (*i)->name;
        delete (*i);
    }
    layers.clear();
}

Tile *Level::GetTile(int layerId, int xTile, int yTile)
{
    Layer *layer = layers[layerId];   
    if (xTile >= 0 && yTile >= 0 && xTile < (int)layer->width && yTile < (int)layer->height)
        return &layer->tiles[yTile * layers[layerId]->width + xTile];
    else
    {
        if (xTile >= (int)layers[layerId]->width && layer->enableMirrorX == true)
        {
            if (yTile >= (int)layer->height)
            {
                if (layer->enableMirrorY == false)
                    return 0;
                else
                    yTile %= layers[layerId]->height;
            }
            xTile %= layers[layerId]->width;
            return &layer->tiles[yTile * layers[layerId]->width + xTile];
        }
        else if (yTile >= (int)layers[layerId]->height && layer->enableMirrorY == true)
        {
            if (xTile < (int)layer->width)
            {
                yTile %= layers[layerId]->height;
                return &layer->tiles[yTile * layers[layerId]->width + xTile];
            }
        }
        return 0;
    }
}

void Level::Tick(float timeDelta)
{
    for (vector<Layer *>::iterator i = layers.begin(); i != layers.end(); i++)
    {
        Layer *layer = (*i);
        layer->autoScrollX += layer->autoScrollXSpeed * timeDelta;
        layer->autoScrollY += layer->autoScrollYSpeed * timeDelta;
    }
}