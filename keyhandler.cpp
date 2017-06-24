#include "keyhandler.h"
#include "engine.h"

KeyHandler::KeyHandler()
{
}

KeyHandler::~KeyHandler()
{
}

void KeyHandler::Tick(float timeDelta)
{
    Camera *camera = Engine::GetSingleton().GetCamera();

    camera->SetVelocityX(0.0f);
    camera->SetVelocityY(0.0f);

    if (GetKeyState(VK_LEFT) & 0x80)
        camera->SetVelocityX(-1000.0f);
    if (GetKeyState(VK_RIGHT) & 0x80)
        camera->SetVelocityX(1000.0f);
    if (GetKeyState(VK_UP) & 0x80)
        camera->SetVelocityY(-1000.0f);
    if (GetKeyState(VK_DOWN) & 0x80)
        camera->SetVelocityY(1000.0f);
}