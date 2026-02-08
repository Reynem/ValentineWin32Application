#pragma once
#include <windows.h>

void MoveWindowSmooth(HWND hWnd, int targetX, int targetY, int steps = 50, int delayMs = 10);
void DrawHeart(HWND hWnd, HDC hdc, COLORREF color);