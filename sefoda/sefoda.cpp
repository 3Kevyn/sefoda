#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "utils.h"
#include "winput.h"

using std::cout, std::endl;

HWND hwndJogo = NULL;
COLORREF TargetColor = NULL;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    wchar_t windowTitle[256];
    GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));

    std::wstring title = windowTitle;
    if (title.find(L"otPokemon") == 0)
    {
        *(HWND*)lParam = hwnd;
        return FALSE;
    }
    return TRUE;
}

void MonitorWindow()
{
    if (hwndJogo == NULL || !IsWindow(hwndJogo))
    {
        if (hwndJogo == NULL || !IsWindow(hwndJogo))
        {
            hwndJogo = NULL;
            EnumWindows(EnumWindowsProc, (LPARAM)&hwndJogo);
        }
        Sleep(1000); 
    }
}

void VerificarTeclas(HWND hwnd)
{
    if (hwnd == NULL || !IsWindow(hwnd)) return;
    if (GetForegroundWindow() != hwnd  || CheckChatOpen(hwnd)) return;


    if (GetAsyncKeyState(69) & 0x8000) // tecla 'E'
    {
        if (CheckBattleAndTarget(hwnd))
        {
            Sleep(100);
        }
        FullHit(hwnd);
    }

    if (GetAsyncKeyState(VK_DELETE) & 0x8000) // pesca 
    {
        while (!(GetAsyncKeyState(VK_ESCAPE)))
        {
            if (!CheckBattle(hwnd))
            {
                pressKey(hwnd, VK_F1);
                Sleep(50);
                continue;
            }
            Sleep(200);
            FullHit(hwnd);
            
        }
    }
    if (GetAsyncKeyState(VK_CAPITAL) & 0x8000) // hit constante 
    {
        while (!(GetAsyncKeyState(VK_ESCAPE)))
        {
            if (!CheckBattleAndTarget(hwnd))
            {
                FullHit(hwnd);
                Sleep(200);
            }
            
        }
    }

    if (GetAsyncKeyState(VK_HOME) & 0x8000) // KS 
    {
        while (!(GetAsyncKeyState(VK_ESCAPE)))
        {
            if (!CheckBattleAndTarget(hwnd))
            {
                pressKey(hwnd, VK_F8);
                Sleep(3);
                continue;
            }
            pressKey(hwnd, VK_F4);
            pressKey(hwnd, VK_F3);
            Sleep(20);
        }
    }

    if (GetAsyncKeyState(VK_SNAPSHOT) & 0x8000) // mansion
    {
        while (!(GetAsyncKeyState(VK_ESCAPE)))
        {
            AutoDex(hwnd);
            Sleep(150);
        }
        Sleep(50);
    }

    if (GetAsyncKeyState(VK_F12) & 0x8000)//quest Crystais
    {
        
        POINT point;
        GetCursorPos(&point);
        TargetColor = GetColorAtMouse(hwndJogo);
        std::cout << "cor salva" << std::endl;
        while (!(GetAsyncKeyState(VK_ESCAPE)))
        {
            if (GetAsyncKeyState(VK_F11) & 0x8000)
            {
                COLORREF CorAtual = GetColorAtMouse(hwndJogo);
                
                if (IsColorSimilar(CorAtual, TargetColor,50))
                {
                    std::cout << "clicando" << std::endl;
                    
                   WInput::SimulateMouseClickAtR(hwnd, point.x, point.y);
                    Sleep(30);
                }
            }
            else if (GetAsyncKeyState(VK_ESCAPE) & 1)
            {
                TargetColor = NULL;
            }
            Sleep(100);
        }
        std::cout << "apagando cor" << std::endl;
        TargetColor = NULL;
    }
    /*
    if (GetAsyncKeyState(0x43) & 0x8000)
    {
        POINT point;
        GetCursorPos(&point);
        std::cout << "posicao = "<< point.x << " ," << point.y << std::endl;
        COLORREF CorAtual = GetColorAtMouse(hwndJogo);
        std::cout << "cor  R= " << (int)GetRValue(CorAtual)<<" G="<< (int)GetGValue(CorAtual)<<" B="<< (int)GetBValue(CorAtual)  << std::endl;

    }
    */
    
}

int main()
{
    while (true)
    {
        if (hwndJogo && IsWindow(hwndJogo))
        {
            VerificarTeclas(hwndJogo);
        }
        else
        {
            DetectarResolucao();
            while (hwndJogo == NULL || !IsWindow(hwndJogo))
            {
                MonitorWindow();
                Sleep(1000); 
            }
        }

        Sleep(50);
    }
    return 0;
}
