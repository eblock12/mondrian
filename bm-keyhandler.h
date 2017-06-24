#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "object.h"
#include "bm-player.h"

namespace BlasterMaster
{
    class KeyHandler : public Object
    {
    public:
        KeyHandler();
        ~KeyHandler();
        void SetCurrentPlayer(Player *newPlayer) { player = newPlayer; }
        void Tick(float timeDelta);
    private:
        Player *player;
        bool allowFire;
    };
}