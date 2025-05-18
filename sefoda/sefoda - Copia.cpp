#include <windows.h>
#include <iostream>
#include <string>
#include "winput.h"

#define ID_LOOT 1
#define ID_FULLHIT 2
#define ID_PESCAS 3
#define ID_TARGET 4

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    wchar_t windowTitle[256];
    GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));

    std::wstring title = windowTitle;
    if (title.find(L"otPokemon") == 0)
    {
        HWND* pFoundWindow = (HWND*)lParam;
        *pFoundWindow = hwnd;
        return FALSE;
    }
    return TRUE;
}

void SimulateMouseClickAt(HWND hwnd, int x, int y)
{
    POINT pt = { x, y };
    ClientToScreen(hwnd, &pt);
    POINT originalPos;
    GetCursorPos(&originalPos);
    WInput::SendClick(pt);
    SetCursorPos(originalPos.x, originalPos.y);
}

void SimulateMouseClickAtR(HWND hwnd, int x, int y)
{
    POINT pt = { x, y };
    ClientToScreen(hwnd, &pt);
    POINT originalPos;
    GetCursorPos(&originalPos);
    WInput::SendClickRight(pt);
    SetCursorPos(originalPos.x, originalPos.y);
}

bool checkBattleT(HWND hwnd)
{
    int targetX = 29, targetY = 122;
    int targetAlvoX = 26, targetAlvoY = 120;
    COLORREF targetColor = RGB(0, 0, 0);
    COLORREF colorTarget = RGB(255,0,0);
    HDC hdc = GetDC(hwnd);
    if (!hdc)
        return false;

    COLORREF color = GetPixel(hdc, targetX, targetY);
    bool inBattle = (GetRValue(color) == GetRValue(targetColor) &&
        GetGValue(color) == GetGValue(targetColor) &&
        GetBValue(color) == GetBValue(targetColor));

    COLORREF colorBattle = GetPixel(hdc, targetAlvoX, targetAlvoY);
    bool inBattle1 = (GetRValue(colorBattle) == GetRValue(colorTarget) &&
        GetGValue(colorBattle) == GetGValue(colorTarget) &&
        GetBValue(colorBattle) == GetBValue(colorTarget));

    if (inBattle && !inBattle1)
    {
        SimulateMouseClickAt(hwnd, targetX, targetY);
        Sleep(150);
    }
    else (!inBattle);
    return inBattle1;
    ReleaseDC(hwnd, hdc);
}

bool checkBattle(HWND hwnd)
{
    int targetX = 29, targetY = 122;
    COLORREF targetColor = RGB(0, 0, 0);

    HDC hdc = GetDC(hwnd);
    if (!hdc)
        return false;

    COLORREF color = GetPixel(hdc, targetX, targetY);
    bool inBattle = (GetRValue(color) == GetRValue(targetColor) &&
        GetGValue(color) == GetGValue(targetColor) &&
        GetBValue(color) == GetBValue(targetColor));
    ReleaseDC(hwnd, hdc);
    return inBattle;
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

bool CheckChatOpen(HWND hwnd)
{
    int targetX = 1680, targetY = 1006;
    int minR = 200, maxR = 225;
    int minG = 173, maxG = 180;
    int minB = 74, maxB = 76;

    HDC hdc = GetDC(hwnd);
    if (hdc)
    {
        COLORREF color = GetPixel(hdc, targetX, targetY);
        int r = GetRValue(color);
        int g = GetGValue(color);
        int b = GetBValue(color);
        ReleaseDC(hwnd, hdc);

        // Verifica se a cor está dentro do intervalo
        if ((r >= minR && r <= maxR) &&
            (g >= minG && g <= maxG) &&
            (b >= minB && b <= maxB))
        {
            std::cout << "Chat aberto" << std::endl;
            return true;
        }
    }

    return false;
}

void AtualizarHotkeys(HWND hwnd)
{
    static bool chatAberto = false;
    bool chatAtualmenteAberto = CheckChatOpen(hwnd);
    bool janelaAtiva = (GetForegroundWindow() == hwnd);

    if (chatAtualmenteAberto || !janelaAtiva)
    {
        if (!chatAberto)  // Apenas desregistrar se ainda não estiver desregistrado
        {
            std::cout << "Desativando hotkeys..." << std::endl;
            UnregisterHotKey(0, ID_LOOT);
            UnregisterHotKey(0, ID_FULLHIT);
            UnregisterHotKey(0, ID_PESCAS);
            UnregisterHotKey(0, ID_TARGET);
            chatAberto = true;
        }
    }
    else
    {
        if (chatAberto)  // Apenas registrar se ainda não estiver registrado
        {
            std::cout << "Reativando hotkeys..." << std::endl;
            RegisterHotKey(0, ID_LOOT, 0, 82);
            RegisterHotKey(0, ID_FULLHIT, 0, 69);
            RegisterHotKey(0, ID_PESCAS, 0, VK_DELETE);
            RegisterHotKey(0, ID_TARGET, 0, VK_HOME);
            chatAberto = false;
        }
    }
}


void Loot(HWND hwnd)
{
    if (hwnd == NULL)
    {
        return;
    }
    if (GetForegroundWindow() != hwnd)
    {
        return;
    }
    SimulateMouseClickAtR(hwnd, 950, 500);
    Sleep(100);
    if (!checkLootbox(hwnd))
    {
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        SimulateMouseClickAt(hwnd, 1743, 404);
        Sleep(10);
    }
    SimulateMouseClickAt(hwnd, 1906, 371);
}

void pressKey(HWND hwnd, int key)
{
    PostMessageA(hwnd, WM_KEYDOWN, key, 0);
    PostMessageA(hwnd, WM_KEYUP, key, 0);
}

void FullHitInGame(HWND hwnd)
{
    EnumWindows(EnumWindowsProc, (LPARAM)&hwnd);
    if (GetForegroundWindow() != hwnd)
    {
        return;
    }
    if (CheckChatOpen(hwnd))
        return;
    for (int i = VK_F1; i <= VK_F5; ++i)
    {
        pressKey(hwnd, i);
    }
}
void FullHit(HWND hwnd)
{
    EnumWindows(EnumWindowsProc, (LPARAM)&hwnd);
    if (CheckChatOpen(hwnd))
        return;
    for (int i = VK_F1; i <= VK_F5; ++i)
    {
        pressKey(hwnd, i);
    }
}

int main()
{
    MSG msg = {};
    HWND hwndJogo = NULL;
    RegisterHotKey(0, ID_LOOT, 0, 82);
    RegisterHotKey(0, ID_FULLHIT, 0, 69);
    RegisterHotKey(0, ID_PESCAS, 0, VK_DELETE);
    RegisterHotKey(0, ID_TARGET, 0, VK_HOME);
    EnumWindows(EnumWindowsProc, (LPARAM)&hwndJogo);
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {

        if (msg.message == WM_HOTKEY)
        {
            if (CheckChatOpen(hwndJogo))
                continue;
            AtualizarHotkeys(hwndJogo);
            switch (msg.message)
            {
            case WM_HOTKEY:
                switch (msg.wParam)
                {
                case ID_LOOT:
                    Loot(hwndJogo);
                    break;
                case ID_FULLHIT:
                    if (!checkBattleT(hwndJogo))
                    {
                        FullHitInGame(hwndJogo);
                    }
                    FullHit(hwndJogo);
                    break;
                case ID_PESCAS:
                    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
                    {
                        if (!checkBattle(hwndJogo))
                        {
                            continue;
                        }
                        FullHit(hwndJogo);
                        Sleep(100);
                    }
                    break;
                case ID_TARGET:
                    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
                    {
                        if (checkBattleT(hwndJogo))
                        {
                            Sleep(80);
                            FullHit(hwndJogo);
                            break;
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    return (int)msg.message;
}
