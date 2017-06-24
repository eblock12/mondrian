#include "application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Application *app = new Application(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    int retCode = app->Execute();   
    delete app;
    return retCode;
}