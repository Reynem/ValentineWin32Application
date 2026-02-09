#include "windows_init.h"
#include "Project1.h"
#include "utils_windows.h"
#include <cmath>

struct AnimationState {
    HWND hWnd;
    double currentX;
    double currentY;
    double stepX;
    double stepY;
    int width;
    int height;
    int targetX;
    int targetY;
};

// Method was changed to make movement cubic and not linear
void MoveAllWindowsParallel(HWND* hWnds, WindowRect* targets, int count, int steps = 50, int delayMs = 10)
{
    if (count <= 0) return;

    // 1. Подготовка: вычисляем шаги для каждого окна
    AnimationState* states = new AnimationState[count];

    for (int i = 0; i < count; i++) {
        states[i].hWnd = hWnds[i];

        RECT rect;
        GetWindowRect(hWnds[i], &rect);

        states[i].width = rect.right - rect.left;
        states[i].height = rect.bottom - rect.top;
        states[i].currentX = (double)rect.left;
        states[i].currentY = (double)rect.top;
        states[i].targetX = targets[i].x;
        states[i].targetY = targets[i].y;

        // Вычисляем приращение (шаг) за одну итерацию
        states[i].stepX = targets[i].x - rect.left;
        states[i].stepY = targets[i].y - rect.top;
    }

    // 2. Главный цикл анимации
    for (int s = 0; s < steps; s++)
    {
        double t = (double)s / (steps - 1);

        // Cubic ease-out
        double k = 1.0 - pow(1.0 - t, 3.0);

        // Сдвигаем ВСЕ окна на один маленький шаг
        for (int i = 0; i < count; i++)
        {
            double x = states[i].currentX + states[i].stepX * k;
            double y = states[i].currentY + states[i].stepY * k;

            MoveWindow(
                states[i].hWnd,
                (int)x,
                (int)y,
                states[i].width,
                states[i].height,
                TRUE
            );
        }

        // Ждем один раз для всех окон
        Sleep(delayMs);
    }

    // 3. Финальная коррекция (чтобы точно попасть в координаты)
    for (int i = 0; i < count; i++)
    {
        MoveWindow(
            states[i].hWnd,
            states[i].targetX,
            states[i].targetY,
            states[i].width,
            states[i].height,
            TRUE
        );
    }

    delete[] states;
}


void SpiralToCenter(
    HWND* hWnds,
    int count,
    int centerX,
    int centerY,
    int steps = 240,
    int delayMs = 10
)
{
    struct SpiralState {
        HWND hWnd;
        double angle;
        double radius;
        int width;
        int height;
    };

    SpiralState* states = new SpiralState[count];

    // Initialization
    for (int i = 0; i < count; i++)
    {
        RECT rc;
        GetWindowRect(hWnds[i], &rc);

        int w = rc.right - rc.left;
        int h = rc.bottom - rc.top;

        double cx = rc.left + w / 2.0;
        double cy = rc.top + h / 2.0;

        double dx = cx - centerX;
        double dy = cy - centerY;

        states[i].hWnd = hWnds[i];
        states[i].width = w;
        states[i].height = h;
        states[i].radius = sqrt(dx * dx + dy * dy);
        states[i].angle = atan2(dy, dx); // current angle
    }

    // 2. Animation
    for (int s = 0; s < steps; s++)
    {
        for (int i = 0; i < count; i++)
        {
            // Reducing radius
            states[i].radius *= 0.98;

            // Rotating по часовой стрелке
            states[i].angle += 0.05;

            double x = centerX +
                states[i].radius * cos(states[i].angle) -
                states[i].width / 2.0;

            double y = centerY +
                states[i].radius * sin(states[i].angle) -
                states[i].height / 2.0;

            MoveWindow(
                states[i].hWnd,
                (int)x,
                (int)y,
                states[i].width,
                states[i].height,
                TRUE
            );
        }

        Sleep(delayMs);
    }

    // 3. Final positioning in center
    for (int i = 0; i < count; i++)
    {
        MoveWindow(
            states[i].hWnd,
            centerX - states[i].width / 2,
            centerY - states[i].height / 2,
            states[i].width,
            states[i].height,
            TRUE
        );
    }

    delete[] states;
}

enum {
	PINK_C = RGB(255, 192, 203),
	RED_C = RGB(255, 0, 0),
	CYAN_C = RGB(0, 255, 255),
	LIME_C = RGB(191, 255, 0),
	BLUE_C = RGB(15, 15, 255),
	YELLOW_C = RGB(255, 255, 0)
};

COLORREF heartColors[NUM_ROWSS * NUM_COLSS]{
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

    const int rows = NUM_ROWSS;
    const int cols = NUM_COLSS;
    const int numWindows = NUM_WINDOWS;

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
            WS_EX_LAYERED | WS_EX_TOPMOST,
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
		SetPropW(hWnd[i], L"WindowType", (HANDLE)0);

        ShowWindow(hWnd[i], nCmdShow);
        UpdateWindow(hWnd[i]);
    }

    WindowRect newPositions[numWindows];
    for (int i = 0; i < 6; i++) {
        RECT rc;
        GetWindowRect(hWnd[i], &rc);
        newPositions[i].x = rc.left + 50;
        newPositions[i].y = rc.top + 50;
    }

    const int halfNumWindows = (int)numWindows / 2;

    for (int i = 0; i < numWindows; i++) {
        RECT rc;
        GetWindowRect(hWnd[i], &rc);
        if (i % 2 == 0) {
            newPositions[i].x = rc.left - 250;
            newPositions[i].y = rc.top;
        }
        else {
            newPositions[i].x = rc.left + 250;
            newPositions[i].y = rc.top;
        }
    }

    MoveAllWindowsParallel(hWnd, newPositions, numWindows, 30, 15);

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    SpiralToCenter(
        hWnd,
        numWindows,
        centerX,
        centerY,
        240,     // steps
        25       // duration
    );

    Sleep(2000);

	for (int i = 0; i < numWindows - 1; i++) {
        if (hWnd[i]) {
			DestroyWindow(hWnd[i]);
			hWnd[i] = nullptr;
        }
    }

	SetPropW(hWnd[numWindows - 1], L"WindowType", (HANDLE)1);

	InvalidateRect(hWnd[numWindows - 1], NULL, TRUE); // Force redraw to show the love letter

	UpdateWindow(hWnd[numWindows - 1]);


    // Opening letter

    Sleep(2000);

    SetPropW(hWnd[numWindows - 1], L"WindowType", (HANDLE)2);

    InvalidateRect(hWnd[numWindows - 1], NULL, TRUE); // Force redraw to show the love letter

    UpdateWindow(hWnd[numWindows - 1]);

    RECT rect;
    GetWindowRect(hWnd[numWindows - 1], &rect);

    MoveWindowSmooth(hWnd[numWindows - 1], rect.left, rect.top + 50);

    return TRUE;
}