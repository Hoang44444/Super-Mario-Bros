#include "WinApp.h"
#include "Renderer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WinApp myApp;
    if (myApp.Initialize(hInstance, 1280, 720)) {
        return myApp.Run();
    }

    return 0;
}