#pragma once
#include <windows.h>

void MoveWindowSmooth(HWND hWnd, int targetX, int targetY, int steps = 50, int delayMs = 10);
void DrawHeart(HWND hWnd, HDC hdc, COLORREF color);
void DrawLoveLetter(HWND hWnd, HDC hdc);
void DrawOpenEnvelope(HWND hWnd, HDC hdc);
void DrawLetter(HWND hWnd, HDC hdc, const wchar_t* myLoveWishings);

extern struct WindowRect {
    int x;
    int y;
    int width;
    int height;
};