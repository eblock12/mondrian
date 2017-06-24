#pragma once

#include "object.h"
#include "bm-player.h"
#include "bm-keyhandler.h"

namespace BlasterMaster
{
    const int SM_PLAYER = 0;
    const int SC_NORA = 0;
    const int SC_BULLET = 1;

    class GameHandler : public Object
    {
    public:
        GameHandler();
        ~GameHandler();
    private:
        Player *player;
    };
}