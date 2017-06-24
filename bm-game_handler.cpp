#include "bm-game_handler.h"
#include "application.h"
#include "engine.h"

using namespace BlasterMaster;

GameHandler::GameHandler()
{
    Engine *engine = Engine::GetSingletonPtr();
    Application *app = Application::GetSingletonPtr();

    engine->SetLevelFileName(".\\level1.bml");
    if (engine->LoadLevel() == false)
        app->Exit();

    if (engine->LoadSpriteMap("bm-sprites1.tga") == false)
        app->Exit();

    SpriteMap *smPlayer = engine->GetSpriteMap(SM_PLAYER);
    smPlayer->AddTextureCoords(0, 0, 50, 31); // SC_NORA
    smPlayer->AddTextureCoords(51, 0, 59, 7); // SC_BULLET

    player = new Player();
    engine->AddObject(player);

    KeyHandler *keyHandler = new KeyHandler();
    keyHandler->SetCurrentPlayer(player);
    engine->AddObject(keyHandler);

    app->SetTitle("Blaster Master");
}

GameHandler::~GameHandler()
{
}