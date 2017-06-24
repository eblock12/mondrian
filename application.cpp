#include "application.h"

Application::Application(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    closing = false;
    this->instance = instance;

    WNDCLASSEX mainWindowClass;
    ZeroMemory(&mainWindowClass, sizeof(WNDCLASSEX));
    mainWindowClass.cbSize = sizeof(WNDCLASSEX);
    mainWindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    mainWindowClass.hInstance = instance;
    mainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    mainWindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    mainWindowClass.lpszClassName = MAIN_WINDOW_CLASS_NAME;
    mainWindowClass.lpfnWndProc = (WNDPROC)MessageHandler;

    if (RegisterClassEx(&mainWindowClass) == 0)
    {
        ErrorMsg("Failed to register window class. The application window cannot be created.");
        return;
    }

    int winStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX;

    if (!(windowHandle = CreateWindowEx(0, MAIN_WINDOW_CLASS_NAME, "Mondrian", winStyle, CW_USEDEFAULT, CW_USEDEFAULT, 512, 480, NULL, 0, instance, NULL)))
    {
        ErrorMsg("The application window could not be created due to an unknown internal error.");
        return;
    }

    timeBeginPeriod(1);

    engine = new Engine();
    if (closing == true)
    {
        DestroyWindow(windowHandle);
        return;
    }

    ShowWindow(windowHandle, cmdShow);
}

Application::~Application()
{
    if (engine)
        delete engine;
    timeEndPeriod(1);
}

int Application::Execute()
{
    MSG msg;

    unsigned int timeStart;

    for (;;)
    {
        timeStart = timeGetTime();
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                return (int)msg.wParam;
        }
        engine->Tick();
        engine->timeInterval += (float)timeGetTime() / 1000.0f - (float)timeStart / 1000.0f;
    }
    return (int)msg.wParam;
}

void Application::ErrorMsg(const char *error, ...)
{
    va_list arg_list;
    va_start(arg_list, error);

    char text[1024];
    vsprintf(text, error, arg_list);
    MessageBox(windowHandle, text, "Error", MB_ICONERROR);

    va_end(arg_list);
}

LRESULT Application::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Application *app = Application::GetSingletonPtr();
    switch (msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            app->width = LOWORD(lParam);
            app->height = HIWORD(lParam);
            app->engine->RequestViewportResize(LOWORD(lParam), HIWORD(lParam));
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}