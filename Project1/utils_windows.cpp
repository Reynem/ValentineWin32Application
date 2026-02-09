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

void DrawLoveLetter(HWND hWnd, HDC hdc) {
    if (!hWnd) return;

    // Get window dimensions
    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    int cx = width / 2;
    int cy = height / 2;

    // Calculate envelope dimensions
    int envW = width - 40;
    int envH = height - 60;

    int left = cx - envW / 2;
    int top = cy - envH / 2;
    int right = cx + envW / 2;
    int bottom = cy + envH / 2;

    // Setup colors (off-white to prevent transparency)
    COLORREF paperColor = RGB(250, 245, 255);
    COLORREF lineColor = RGB(220, 20, 60);
    COLORREF heartColor = RGB(255, 0, 50);

    // Draw envelope body
    HBRUSH hPaperBrush = CreateSolidBrush(paperColor);
    HPEN hBorderPen = CreatePen(PS_SOLID, 3, lineColor);

    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hPaperBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, hBorderPen);

    Rectangle(hdc, left, top, right, bottom);

    // Draw fold lines
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, cx, cy);
    LineTo(hdc, right, top);

    MoveToEx(hdc, left, bottom, NULL);
    LineTo(hdc, cx, cy + (envH / 4));

    MoveToEx(hdc, right, bottom, NULL);
    LineTo(hdc, cx, cy + (envH / 4));

    // Prepare for heart seal drawing
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBorderPen);
    DeleteObject(hPaperBrush);

    HBRUSH hHeartBrush = CreateSolidBrush(heartColor);
    HPEN hHeartPen = CreatePen(PS_SOLID, 1, heartColor);

    SelectObject(hdc, hHeartBrush);
    SelectObject(hdc, hHeartPen);

    // Calculate heart seal coordinates
    double size = (double)min(envW, envH) / 6.0;
    int hCx = cx;
    int hCy = cy;

    // Define Bezier points for heart
    POINT pts[7];
    pts[0].x = hCx;                     pts[0].y = hCy - (int)(size * 0.5);
    pts[1].x = hCx + (int)(size * 1.2); pts[1].y = hCy - (int)(size * 1.5);
    pts[2].x = hCx + (int)(size * 2.2); pts[2].y = hCy + (int)(size * 0.5);
    pts[3].x = hCx;                     pts[3].y = hCy + (int)(size * 1.5);
    pts[4].x = hCx - (int)(size * 2.2); pts[4].y = hCy + (int)(size * 0.5);
    pts[5].x = hCx - (int)(size * 1.2); pts[5].y = hCy - (int)(size * 1.5);
    pts[6].x = hCx;                     pts[6].y = hCy - (int)(size * 0.5);

    // Draw heart seal
    BeginPath(hdc);
    PolyBezier(hdc, pts, 7);
    EndPath(hdc);
    FillPath(hdc);

    // Cleanup
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hHeartBrush);
    DeleteObject(hHeartPen);
}

void DrawOpenEnvelope(HWND hWnd, HDC hdc) {
    if (!hWnd) return;

    // Get window dimensions
    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int cx = width / 2;
    int cy = height / 2;

    // Calculate envelope dimensions
    int envW = width - 40;
    int envH = height - 60;
    int left = cx - envW / 2;
    int top = cy - envH / 2;
    int right = cx + envW / 2;
    int bottom = cy + envH / 2;

    // Colors
    COLORREF envelopeColor = RGB(250, 245, 255);
    COLORREF lineColor = RGB(220, 20, 60);
    COLORREF heartColor = RGB(255, 0, 50);

    // Draw envelope body (lower part)
    HBRUSH hEnvBrush = CreateSolidBrush(envelopeColor);
    HPEN hBorderPen = CreatePen(PS_SOLID, 3, lineColor);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hEnvBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, hBorderPen);

    Rectangle(hdc, left, top, right, bottom);

    // Draw fold lines on body
    MoveToEx(hdc, left, bottom, NULL);
    LineTo(hdc, cx, cy + (envH / 4));
    MoveToEx(hdc, right, bottom, NULL);
    LineTo(hdc, cx, cy + (envH / 4));

    // Draw open flap (upper triangular part) - reduced size
    int flapHeight = envH / 5; // Уменьшенная высота клапана
    POINT flap[4];
    flap[0].x = left;
    flap[0].y = top;
    flap[1].x = cx;
    flap[1].y = top - flapHeight; // Клапан открыт вверх, но меньше
    flap[2].x = right;
    flap[2].y = top;
    flap[3].x = left;
    flap[3].y = top;

    Polygon(hdc, flap, 3);

    // Draw fold lines on flap
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, cx, top - flapHeight);
    LineTo(hdc, right, top);

    // Draw heart seal on flap
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBorderPen);
    DeleteObject(hEnvBrush);

    HBRUSH hHeartBrush = CreateSolidBrush(heartColor);
    HPEN hHeartPen = CreatePen(PS_SOLID, 1, heartColor);
    SelectObject(hdc, hHeartBrush);
    SelectObject(hdc, hHeartPen);

    // Heart on flap - adjusted position
    double size = (double)min(envW, envH) / 10.0; // Немного меньше сердце
    int hCx = cx;
    int hCy = top - flapHeight / 2; // Центр клапана

    POINT pts[7];
    pts[0].x = hCx;                     pts[0].y = hCy - (int)(size * 0.5);
    pts[1].x = hCx + (int)(size * 1.2); pts[1].y = hCy - (int)(size * 1.5);
    pts[2].x = hCx + (int)(size * 2.2); pts[2].y = hCy + (int)(size * 0.5);
    pts[3].x = hCx;                     pts[3].y = hCy + (int)(size * 1.5);
    pts[4].x = hCx - (int)(size * 2.2); pts[4].y = hCy + (int)(size * 0.5);
    pts[5].x = hCx - (int)(size * 1.2); pts[5].y = hCy - (int)(size * 1.5);
    pts[6].x = hCx;                     pts[6].y = hCy - (int)(size * 0.5);

    BeginPath(hdc);
    PolyBezier(hdc, pts, 7);
    EndPath(hdc);
    FillPath(hdc);

    // Cleanup
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hHeartBrush);
    DeleteObject(hHeartPen);
}

void DrawLetter(HWND hWnd, HDC hdc, const wchar_t* myLoveWishings) {
    if (!hWnd) return;

    // Get window dimensions
    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int cx = width / 2;
    int cy = height / 2;

    // Calculate letter dimensions (slightly smaller than envelope and peeking out)
    int letterW = width;
    int letterH = height;
    int left = cx - letterW / 2;
    int top = cy - letterH / 2 + 10; // Немного выглядывает
    int right = cx + letterW / 2;
    int bottom = cy + letterH / 2 - 20;

    // Colors
    COLORREF paperColor = RGB(255, 220, 230); // Светло-розовый
    COLORREF textColor = RGB(100, 50, 150);

    // Draw letter paper
    HBRUSH hPaperBrush = CreateSolidBrush(paperColor);
    HPEN hPaperPen = CreatePen(PS_SOLID, 2, RGB(255, 180, 200));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hPaperBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, hPaperPen);

    Rectangle(hdc, left, top, right, bottom);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hPaperPen);
    DeleteObject(hPaperBrush);

    // Setup text drawing
    RECT textRect;
    textRect.left = left + 20;
    textRect.top = top + 5;
    textRect.right = right - 20;
    textRect.bottom = bottom - 20;

    // Set text properties
    SetTextColor(hdc, textColor);
    SetBkMode(hdc, TRANSPARENT);

    // Create and select font
    HFONT hFont = CreateFont(
        18,                        // Height
        0,                         // Width
        0,                         // Escapement
        0,                         // Orientation
        FW_NORMAL,                 // Weight
        FALSE,                     // Italic
        FALSE,                     // Underline
        FALSE,                     // StrikeOut
        DEFAULT_CHARSET,           // CharSet
        OUT_DEFAULT_PRECIS,        // OutPrecision
        CLIP_DEFAULT_PRECIS,       // ClipPrecision
        ANTIALIASED_QUALITY,       // Quality (улучшенное качество)
        DEFAULT_PITCH | FF_SWISS,  // PitchAndFamily
        L"Georgia"                   // Font name
    );

    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    // Draw the text from myLoveWishings variable
    if (myLoveWishings != NULL) {
        DrawText(hdc, myLoveWishings, -1, &textRect,
            DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX);
    }

    // Cleanup
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}