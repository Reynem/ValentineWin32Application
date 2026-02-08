#include "windows_init.h"
#include "Project1.h"
#include "utils_windows.h"

struct WindowRect {
    int x;
    int y;
    int width;
    int height;
};

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    const int rows = 3;
    const int cols = 2;
    const int numWindows = rows * cols;

    WindowRect pack_of_values[numWindows];

    int winWidth = 300;
    int winHeight = 200;
    int spacingX = 20;
    int spacingY = 20;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int totalWidth = cols * winWidth + (cols - 1) * spacingX;
    int totalHeight = rows * winHeight + (rows - 1) * spacingY;

    int startX = (screenWidth - totalWidth) / 2;
    int startY = (screenHeight - totalHeight) / 2;

	// Calculate positions and sizes for each window
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int idx = r * cols + c;
            pack_of_values[idx].x = startX + c * (winWidth + spacingX);
            pack_of_values[idx].y = startY + r * (winHeight + spacingY);
            pack_of_values[idx].width = winWidth;
            pack_of_values[idx].height = winHeight;
        }
    }

    HWND hWnd[numWindows];

    for (int i = 0; i < numWindows; i++)
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

	MoveWindowSmooth(hWnd[0], 100, 0);

    return TRUE;
}