#include <windows.h>

void MoveWindowSmooth(HWND hWnd, int targetX, int targetY, int steps = 50, int delayMs = 10)
{
    if (!hWnd) return;

	// Get current window position and size
    RECT rect;
    GetWindowRect(hWnd, &rect);

    int startX = rect.left;
    int startY = rect.top;

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

	// calculate step increments
    double stepX = (targetX - startX) / (double)steps;
    double stepY = (targetY - startY) / (double)steps;

    double currentX = startX;
    double currentY = startY;

    for (int i = 0; i < steps; i++)
    {
        currentX += stepX;
        currentY += stepY;

        MoveWindow(hWnd, (int)currentX, (int)currentY, width, height, TRUE);

		Sleep(delayMs); // Smooth delay between movements
    }

    MoveWindow(hWnd, targetX, targetY, width, height, TRUE);
}
