#include "bm-projectile.h"
#include "bm-game_handler.h"
#include "renderer.h"
#include "engine.h"
#include "application.h"

using namespace BlasterMaster;

Projectile::Projectile(float x, float y, float velocityX, float velocityY, Object *owner, int type)
{
    positionX = x;
    positionY = y;
    this->velocityX = velocityX;
    this->velocityY = velocityY;
    this->owner = owner;
    this->type = type;

    if (type == PROJECTILE_NORA_BULLET)
    {
        sprite = new Sprite(SM_PLAYER, SC_BULLET, SC_BULLET_WIDTH, SC_BULLET_HEIGHT);
        Renderer::GetSingleton().AddSprite(sprite);
    }
}

Projectile::~Projectile()
{
    Renderer::GetSingleton().DeleteSprite(sprite);
    delete sprite;
}

void Projectile::Tick(float timeDelta)
{
    Engine *engine = Engine::GetSingletonPtr();
    Application *app = Application::GetSingletonPtr();

    int newX = int(positionX + SC_BULLET_WIDTH / 2 + velocityX * timeDelta);
    int newY = int(positionY + SC_BULLET_HEIGHT / 2 + velocityY * timeDelta);

    if (engine->isColliding(newX, newY) == true || Renderer::GetSingleton().inViewport(newX, newY) == false)
    {
        SelfTerminate();
        if (type == PROJECTILE_NORA_BULLET)
            dynamic_cast<Player *>(owner)->BulletDestroyed();
    }
    else
    {
        positionX += velocityX * timeDelta;
        positionY += velocityY * timeDelta;
    }

    sprite->SetPosition((int)positionX, (int)positionY);
    if (velocityX < 0.0f)
        sprite->SetAttributes(SPRITE_FLIP_HORIZONTAL);
    else
        sprite->SetAttributes(SPRITE_NONE);
}