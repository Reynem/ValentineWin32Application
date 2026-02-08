#include "windows_init.h"
#include "Project1.h"

struct WindowRect {
    int x;
    int y;
    int width;
    int height;
};

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    WindowRect pack_of_values[] = {
        {100, 100, 300, 200},
        {420, 100, 300, 200},
        {100, 320, 300, 200},
        {420, 320, 300, 200}
    };

    HWND hWnd[4];

    for (int i = 0; i < 4; i++)
    {
        hWnd[i] = CreateWindowW(
            szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
            pack_of_values[i].x,
            pack_of_values[i].y,
            pack_of_values[i].width,
            pack_of_values[i].height,
            nullptr, nullptr, hInstance, nullptr
        );

        if (!hWnd[i])
            return FALSE;

        ShowWindow(hWnd[i], nCmdShow);
        UpdateWindow(hWnd[i]);
    }

    return TRUE;
}