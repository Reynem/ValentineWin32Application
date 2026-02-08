#include "windows_init.h"
#include "Project1.h"
#include "utils_windows.h"

struct WindowRect {
    int x;
    int y;
    int width;
    int height;
};

enum {
	NUM_ROWS = 3,
	NUM_COLS = 2,
	PINK_C = RGB(255, 192, 203),
	RED_C = RGB(255, 0, 0),
	CYAN_C = RGB(0, 255, 255),
	LIME_C = RGB(191, 255, 0),
	BLUE_C = RGB(15, 15, 255),
	YELLOW_C = RGB(255, 255, 0)
};

COLORREF heartColors[NUM_ROWS * NUM_COLS]{
    PINK_C,
    RED_C,
    CYAN_C,
    LIME_C,
    BLUE_C,
    YELLOW_C
};

COLORREF transparentColor = RGB(255, 255, 255);

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
        hWnd[i] = CreateWindowExW(
            WS_EX_LAYERED,
            szWindowClass, szTitle, WS_POPUP,
            pack_of_values[i].x,
            pack_of_values[i].y,
            pack_of_values[i].width,
            pack_of_values[i].height,
            nullptr, nullptr, hInstance, nullptr
        );

        if (!hWnd[i])
            return FALSE;

		SetLayeredWindowAttributes(hWnd[i], transparentColor, 0, LWA_COLORKEY);
		SetPropW(hWnd[i], L"HeartColor", (HANDLE)heartColors[i]);

        ShowWindow(hWnd[i], nCmdShow);
        UpdateWindow(hWnd[i]);
    }

    return TRUE;
}