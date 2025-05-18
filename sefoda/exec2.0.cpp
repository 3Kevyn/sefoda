#include <windows.h>
#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>
#include "winput.h"

bool isKeyPressed(int key)
{
    return GetAsyncKeyState(key) & 0x8000;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    wchar_t windowTitle[256];
    GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));

    std::wstring title = windowTitle;
    if (title.find(L"otPokemon") == 0)
    {
        HWND *pFoundWindow = (HWND *)lParam;
        *pFoundWindow = hwnd;
        return FALSE;
    }
    return TRUE;
}

void SimulateMouseClickAt(HWND hwnd, int x, int y)
{
    SetForegroundWindow(hwnd);
    Sleep(50);
    POINT pt = {x, y};
    ClientToScreen(hwnd, &pt);
    POINT originalPos;
    GetCursorPos(&originalPos);
    WInput::SendClick(pt);
    SetCursorPos(originalPos.x, originalPos.y);
}

void SimulateMouseClickAtR(HWND hwnd, int x, int y)
{
    SetForegroundWindow(hwnd);
    Sleep(50);
    POINT pt = {x, y};
    ClientToScreen(hwnd, &pt);
    POINT originalPos;
    GetCursorPos(&originalPos);
    WInput::SendClickRight(pt);
    SetCursorPos(originalPos.x, originalPos.y);
}

bool checkLootbox(HWND hwnd)
{
    int targetX = 1904, targetY = 370;
    COLORREF targetColor = RGB(224, 179, 75);

    HDC hdc = GetDC(hwnd);
    if (hdc)
    {
        COLORREF color = GetPixel(hdc, targetX, targetY);
        int r = GetRValue(color);
        int g = GetGValue(color);
        int b = GetBValue(color);

        if (r == GetRValue(targetColor) && g == GetGValue(targetColor) && b == GetBValue(targetColor))
        {
            ReleaseDC(hwnd, hdc);
            return true;
        }

        ReleaseDC(hwnd, hdc);
    }

    return false;
}

void manipularJanela(HWND hwnd)
{
    if (hwnd != NULL)
    {
        SimulateMouseClickAtR(hwnd, 950, 500);
        Sleep(200);

        if (checkLootbox(hwnd))
        {
            for (int i = 0; i < 4; i++)
            {
                SimulateMouseClickAt(hwnd, 1743, 404);
                Sleep(40);
            }
            SimulateMouseClickAt(hwnd, 1906, 371);
        }
    }
}

void pressKey(HWND hwnd, int key)
{
    PostMessageA(hwnd, WM_KEYDOWN, key, 0);
    PostMessageA(hwnd, WM_KEYUP, key, 0);
}

int main()
{
    WInput::ConfigInput(KEY_VIRTUAL);
    MSG msg = {};
    bool repetir = false;
    bool pausado = false;
    HWND hwndJogo = NULL;

    EnumWindows(EnumWindowsProc, (LPARAM)&hwndJogo);

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (isKeyPressed(VK_RETURN) && GetForegroundWindow() == hwndJogo)
        {
            pausado = !pausado;
            Sleep(100);
        }

        if (pausado)
        {
            Sleep(10);
            continue;
        }

        if (isKeyPressed(VK_OEM_4) && GetForegroundWindow() == hwndJogo)
        {
            repetir = !repetir;
            Sleep(100);
        }

        if (repetir && GetForegroundWindow() == hwndJogo)
        {
            for (int i = VK_F1; i <= VK_F5; ++i)
            {
                pressKey(hwndJogo, i);
            }

            Sleep(50);
        }
        else
        {
            if (GetForegroundWindow() == hwndJogo)
            {
                if (isKeyPressed('R'))
                {
                    manipularJanela(hwndJogo);
                }

                if (isKeyPressed('E'))
                {
                    for (int i = VK_F1; i <= VK_F5; ++i)
                    {
                        pressKey(hwndJogo, i);
                    }
                }
            }

            Sleep(10);
        }
    }

    return 0;
}
