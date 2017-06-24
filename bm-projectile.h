#pragma once

#include "object.h"
#include "sprite.h"

const int SC_BULLET_WIDTH = 8;
const int SC_BULLET_HEIGHT = 8;

enum
{
    PROJECTILE_NORA_BULLET
};

namespace BlasterMaster
{
    class Projectile : public Object
    {
    public:
        Projectile(float x, float y, float velocityX, float velocityY, Object *owner, int type);
        ~Projectile();
        void Tick(float timeDelta);
        void Test() {}
    private:
        Sprite *sprite;
        float positionX;
        float positionY;
        float velocityX;
        float velocityY;
        Object *owner;
        int type;
    };
}