#include "bm-keyhandler.h"

using namespace BlasterMaster;

KeyHandler::KeyHandler()
{
    allowFire = true;
}

KeyHandler::~KeyHandler()
{
}

void KeyHandler::Tick(float timeDelta)
{
    if (GetKeyState(VK_LEFT) & 0x80)
        player->AccelerateLeft(5000.0f * timeDelta);
    else if (GetKeyState(VK_RIGHT) & 0x80)
        player->AccelerateRight(5000.0f * timeDelta);
    else
        player->Brake(1000.0f * timeDelta);
    if (GetKeyState('X') & 0x80)
    {
        player->Jump(3000.0f * timeDelta);
    }
    if ((GetKeyState('Z') & 0x80) && allowFire == true)
    {
        player->FireGuns();
        allowFire = false;
    }
    else if ((GetKeyState('Z') & 0x80) == 0)
        allowFire = true;
}