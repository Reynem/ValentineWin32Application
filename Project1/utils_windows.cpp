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

void DrawHeart(HWND hWnd, HDC hdc, COLORREF color)
{
    if (!hWnd) return;

    // Get window dimensions
    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Center coordinates and base scale
    int cx = width / 2;
    int cy = height / 2;
    int size = min(width, height) / 4;

    // Setup brush and pen (matching pen color removes the black border)
    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    HPEN hPen = CreatePen(PS_SOLID, 1, color);
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    /* Using 2 Bezier curves for the left and right halves.
       Points are calculated to keep the heart symmetric and centered.
    */
    POINT pts[7];
    // Starting point (bottom tip)
    pts[0] = { cx, cy + size };

    // Left side of the heart
    pts[1] = { cx - size * 2, cy - size };     // Control point 1
    pts[2] = { cx - size, cy - size * 2 };     // Control point 2
    pts[3] = { cx, cy - size / 2 };            // Top center notch

    // Right side of the heart
    pts[4] = { cx + size, cy - size * 2 };     // Control point 3
    pts[5] = { cx + size * 2, cy - size };     // Control point 4
    pts[6] = { cx, cy + size };                // Back to start

    // Draw and fill the shape using a Path
    BeginPath(hdc);
    PolyBezier(hdc, pts, 7);
    EndPath(hdc);
    FillPath(hdc);

    // Resource cleanup
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(brush);
    DeleteObject(hPen);
}