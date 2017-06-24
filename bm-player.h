#pragma once

#include "object.h"
#include "sprite.h"

namespace BlasterMaster
{
    const float MAX_VELOCITY_X = 250.0f;
    const float AIR_CONTROL = 0.1f;
    const int NORA_WIDTH = 51;
    const int NORA_HEIGHT = 32;
    const int NORA_TURRET_X = 51;
    const int NORA_TURRET_Y = 4;
    const float NORA_BULLET_FIRE_SPEED = 500.0f;

    class Player : public Object
    {
    public:
        Player();
        ~Player();
        void Tick(float timeDelta);
        void AccelerateLeft(float accel);
        void AccelerateRight(float accel);
        void Brake(float accel);
        void Jump(float accel);
        void FireGuns();
        void BulletDestroyed() { bulletsFired--; }
    private:
        Sprite *sprite;
        float positionX;
        float positionY;
        float velocityX;
        float velocityY;
        float jumpHeight;
        float shotWaitTime;
        int bulletsFired;
    };
}