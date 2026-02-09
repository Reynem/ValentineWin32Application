#pragma once
#include <windows.h>

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

#ifndef WINDOWS_INIT_H
#define WINDOWS_INIT_H

inline constexpr short NUM_ROWSS = 3;
inline constexpr short NUM_COLSS = 2;
inline constexpr short NUM_WINDOWS = NUM_ROWSS * NUM_COLSS;

#endif