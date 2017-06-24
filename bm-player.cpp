#include "bm-player.h"
#include "bm-game_handler.h"
#include "bm-projectile.h"
#include "renderer.h"
#include "engine.h"

using namespace BlasterMaster;

Player::Player()
{
    velocityX = velocityY = 0.0f;
    positionX = 32;
    positionY = 180;

    // Nora Sprite
    sprite = new Sprite(SM_PLAYER, SC_NORA, NORA_WIDTH, NORA_HEIGHT);
    Renderer::GetSingleton().AddSprite(sprite);

    bulletsFired = 0;
    jumpHeight = 0.0f;
    shotWaitTime = 0.0f;
}

Player::~Player()
{
    Renderer::GetSingleton().DeleteSprite(sprite);
    delete sprite;
}

void Player::Tick(float timeDelta)
{
    Engine *engine = Engine::GetSingletonPtr();

    shotWaitTime -= timeDelta;

    velocityY += 1000.0f * timeDelta;

    if (velocityY > 500.0f)
        velocityY = 500.0f;

    if (velocityX < -MAX_VELOCITY_X)
        velocityX = -MAX_VELOCITY_X;
    else if (velocityX > MAX_VELOCITY_X)
        velocityX = MAX_VELOCITY_X;

    if (velocityY < 0.0f)
    {
        jumpHeight -= velocityY * timeDelta;
    }

    float newPositionX = positionX + velocityX * timeDelta;
    float newPositionY = positionY + velocityY * timeDelta;
    if (engine->isColliding((int)newPositionX, (int)positionY) == true || engine->isColliding((int)(newPositionX + NORA_WIDTH - 1), (int)positionY) == true || 
        engine->isColliding((int)newPositionX, (int)(positionY + NORA_HEIGHT - 1)) == true || engine->isColliding((int)(newPositionX + NORA_WIDTH - 1), (int)(positionY + NORA_HEIGHT - 1)) == true)
        velocityX = 0.0f;
    else
        positionX = newPositionX;

    if (engine->isColliding((int)positionX, (int)newPositionY) == true || engine->isColliding((int)positionX, (int)(newPositionY + NORA_HEIGHT - 1)) == true || 
        engine->isColliding((int)(positionX + NORA_WIDTH - 1), (int)newPositionY) == true || engine->isColliding((int)(positionX + NORA_WIDTH - 1), (int)(newPositionY + NORA_HEIGHT - 1)) == true ||
        engine->isColliding((int)(positionX + NORA_WIDTH / 2), (int)newPositionY) == true || engine->isColliding((int)(positionX + NORA_WIDTH / 2), (int)(newPositionY + NORA_HEIGHT - 1)) == true)
    {
        if (velocityY > 0.0f)
            jumpHeight = 0.0f;
        else
            jumpHeight = 90000.0f;
        velocityY = 0.0f;
    }
    else
        positionY = newPositionY;

    sprite->SetPosition((int)positionX, (int)positionY);
    engine->GetCamera()->SetFocus(positionX, positionY);
}

void Player::AccelerateLeft(float accel)
{
    sprite->SetAttributes(SPRITE_FLIP_HORIZONTAL);
    if (jumpHeight == 0.0f)
        velocityX -= accel;
    else
        velocityX -= accel * AIR_CONTROL;
}

void Player::AccelerateRight(float accel)
{
    sprite->SetAttributes(SPRITE_NONE);
    if (jumpHeight == 0.0f)
        velocityX += accel;
    else
        velocityX += accel * AIR_CONTROL;
}

void Player::Brake(float accel)
{
    if (velocityX < 0.0f)
        velocityX += accel;
    else if (velocityX > 0.0f)
        velocityX -= accel;
}

void Player::Jump(float accel)
{
    if (jumpHeight < 60.0f && velocityY <= 0.0f)
        velocityY -= accel;
}

void Player::FireGuns()
{
    if (bulletsFired < 3 && shotWaitTime <= 0.0f)
    {
        if ((sprite->GetAttributes() & SPRITE_FLIP_HORIZONTAL) == SPRITE_FLIP_HORIZONTAL)
            Engine::GetSingleton().AddObject(new Projectile(positionX + NORA_TURRET_X - NORA_WIDTH, positionY + NORA_TURRET_Y, -NORA_BULLET_FIRE_SPEED, 0.0f, this, PROJECTILE_NORA_BULLET));
        else
            Engine::GetSingleton().AddObject(new Projectile(positionX + NORA_TURRET_X, positionY + NORA_TURRET_Y, NORA_BULLET_FIRE_SPEED, 0.0f, this, PROJECTILE_NORA_BULLET));
        bulletsFired++;
        shotWaitTime = 0.1f;
    }
}