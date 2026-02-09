#pragma once
#ifndef PROJECT1_H
#define PROJECT1_H

#define MAX_LOADSTRING 100

extern HINSTANCE hInst;                                // current instance
extern WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
extern WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
inline const wchar_t* myLoveWishings = LR"(Дорогая моя,

Ты - самое прекрасное, что случилось в моей жизни. 
Каждый день с тобой - это подарок. Я люблю тебя всем сердцем!

Твой навсегда ❤)";
#endif

#include "resource.h"
