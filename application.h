#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Mmsystem.h>
#include <cstdarg>
#include <cstdio>

#include "engine.h"
#include "singleton.h"

const char MAIN_WINDOW_CLASS_NAME[] = "MONDRIAN_WINDOW_CLASS";

class Application : public Singleton<Application>
{
public:
    Application(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow);
    ~Application();
    void ErrorMsg(const char *error, ...);
    int Execute();
    void Exit() { closing = true; PostQuitMessage(0); }
    void SetTitle(char *title) { SetWindowText(windowHandle, title); }
    int GetWidth() { return width; }
    int GetHeight() { return height; }
    HWND GetWindowHandle() { return windowHandle; }
private:
    static LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    int width;
    int height;
    bool closing;
    HWND windowHandle;
    HINSTANCE instance;
    Engine *engine;
};