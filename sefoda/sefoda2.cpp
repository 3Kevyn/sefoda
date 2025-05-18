#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "utils.h"

using std::cout, std::endl;


HWND hwndJogo = NULL;

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

void MonitorWindow()
{
        if (hwndJogo == NULL || !IsWindow(hwndJogo)) 
        {
            hwndJogo = NULL; 
            EnumWindows(EnumWindowsProc, 0); 
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
}

void VerificarTeclas(HWND hwnd)
{
    if (hwnd == NULL || !IsWindow(hwnd)) return;

    if (GetForegroundWindow() != hwnd || CheckChatOpen(hwnd)) return;

    if (GetAsyncKeyState(82) & 0x8000) // R
    {
        Loot(hwnd);
    }

    if ((GetAsyncKeyState(69) & 0x8000)) //E
    {
        if (CheckBattleAndTarget(hwnd))
        {
            Sleep(100);
        }
        FullHit(hwnd);
        std::cout << "atacando" << std::endl;
    }

    if (GetAsyncKeyState(VK_DELETE) & 0x8000)
    {
        while (!(GetAsyncKeyState(VK_ESCAPE)))
        {
            if (!CheckBattle(hwnd))
            {
                Sleep(50);
                continue;
            }
            FullHit(hwnd);
            Sleep(100);
        }
    }
    if (GetAsyncKeyState(VK_CAPITAL) & 0x8000) // caps
    {
        while (!(GetAsyncKeyState(VK_ESCAPE)))
        {
            FullHit(hwnd);
            Sleep(100);
            ForceTarget(hwnd);
        }
    }

    if (GetAsyncKeyState(VK_HOME) & 0x8000)
    {
        while (!(GetAsyncKeyState(VK_ESCAPE)))
        {
            if (CheckBattleAndTarget(hwnd))
            {
                Sleep(30);
                FullHit(hwnd);
                break;
            }
            Sleep(5);
        }
    }

    if (GetAsyncKeyState(VK_SNAPSHOT) & 0x8000)
    {
        while (!(GetAsyncKeyState(VK_ESCAPE)))
        {
            AutoDex(hwnd);
            Sleep(150);
        }
        Sleep(50);
    }

    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
    {
        if (GetAsyncKeyState(82) & 0x8000)
        {
            MonitorWindow();
            std::cout << "reload" << std::endl;
            std::cout.flush();
        }
    }

    /*
    if (GetAsyncKeyState(0x54) & 0x8000)  // T
    {
        if (CheckColorInArea(hwnd, 979, 281, 1436, 601, RGB(15, 201, 0))) // verde
        {
            SimulateMouseClickAtR(hwnd, 1318, 570);
            Sleep(50);
        }
    }

    if (GetAsyncKeyState(0x59) & 0x8000) // Y
    {
        if (CheckColorInArea(hwnd, 979, 281, 1436, 601, RGB(28, 100, 231))) // azul escuro
        {
            SimulateMouseClickAtR(hwnd, 1318, 570);
            Sleep(50);
            Break;
        }
    }

    if (GetAsyncKeyState(0x55) & 0x8000) // U
    {
        if (CheckColorInArea(hwnd, 979, 281, 1436, 601, RGB(26, 194, 0))) // azul claro
        {
                SimulateMouseClickAtR(hwnd, 1318, 570);
                Sleep(50);
                break;

        }
    }

    if (GetAsyncKeyState(0x49) & 0x8000) // I
    {

        if (CheckColorInArea(hwnd, 979, 281, 1436, 601, RGB(177, 15, 44))) // vermelho
        {
                SimulateMouseClickAtR(hwnd, 1318, 570);
                Sleep(50);
                break;
        }
    }
    */
}

int main()
{
    std::cout << "depurando" << std::endl;
    while (true)
    {
        VerificarTeclas(hwndJogo);
        Sleep(50);
    }
    return 0;
}
