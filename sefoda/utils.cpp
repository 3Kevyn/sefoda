#include "utils.h"
#include "winput.h"
#include <vector>
#include <string>
#include <iostream>


// 1920x1080
POINT BattleBlackPixel1920{ 29, 122 };
POINT CloseLootBox1920{ 1904, 370 };
POINT PrimeiroSlotDLoot1920{ 1743, 404 };
POINT PesDoBoneco1920{ 950, 500 };
POINT BotaoDeChatAberto1920{ 1679 ,1026 };
POINT InicioListaDeAtk1920{ 8, 99 };
POINT FimListaDeAtk1920{ 178, 330 };


// 1366x768
POINT BattleBlackPixel1366{ 28, 121 };
POINT CloseLootBox1366{ 1378, 394 };
POINT PrimeiroSlotDLoot1366{ 1220, 422 };
POINT PesDoBoneco1366{ 684, 379 };
POINT BotaoDeChatAberto1366{ 1149, 723 };
POINT InicioListaDeAtk1366{ 8, 99 };  
POINT FimListaDeAtk1366{ 150, 250 };  


POINT BattleBlackPixel;
POINT CloseLootBox;
POINT PrimeiroSlotDLoot;
POINT PesDoBoneco;
POINT BotaoDeChatAberto;
POINT InicioListaDeAtk;
POINT FimListaDeAtk;



void DetectarResolucao()
{
    int largura = GetSystemMetrics(SM_CXSCREEN);
    int altura = GetSystemMetrics(SM_CYSCREEN);


    if (largura < 1520 || altura < 880)
    {
        BattleBlackPixel = BattleBlackPixel1366;
        CloseLootBox = CloseLootBox1366;
        PrimeiroSlotDLoot = PrimeiroSlotDLoot1366;
        PesDoBoneco = PesDoBoneco1366;
        BotaoDeChatAberto = BotaoDeChatAberto1366;
        InicioListaDeAtk = InicioListaDeAtk1366;
        FimListaDeAtk = FimListaDeAtk1366;
    }
    else
    {
        BattleBlackPixel = BattleBlackPixel1920;
        CloseLootBox = CloseLootBox1920;
        PrimeiroSlotDLoot = PrimeiroSlotDLoot1920;
        PesDoBoneco = PesDoBoneco1920;
        BotaoDeChatAberto = BotaoDeChatAberto1920;
        InicioListaDeAtk = InicioListaDeAtk1920;
        FimListaDeAtk = FimListaDeAtk1920;
    }
}



void pressKey(HWND hwnd, int key)
{
    PostMessageA(hwnd, WM_KEYDOWN, key, 0);
    PostMessageA(hwnd, WM_KEYUP, key, 0);
}

bool CheckColorInArea(HWND hwnd, int startX, int startY, int width, int height, COLORREF targetColor)
{
    HDC hdcWindow = GetDC(hwnd);
    if (!hdcWindow) return false;

    HDC hdcMem = CreateCompatibleDC(hdcWindow);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcWindow, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    BitBlt(hdcMem, 0, 0, width, height, hdcWindow, startX, startY, SRCCOPY);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    std::vector<BYTE> pixels(width * height * 4);

    if (GetDIBits(hdcMem, hBitmap, 0, height, pixels.data(), &bmi, DIB_RGB_COLORS))
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int index = (y * width + x) * 4;
                BYTE b = pixels[index];
                BYTE g = pixels[index + 1];
                BYTE r = pixels[index + 2];

                if (targetColor == RGB(r, g, b))
                {
                    SelectObject(hdcMem, hOldBitmap);
                    DeleteObject(hBitmap);
                    DeleteDC(hdcMem);
                    ReleaseDC(hwnd, hdcWindow);
                    return true;
                }
            }
        }
    }
    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdcWindow);

    return false;
}

COLORREF GetColorAtMouse(HWND hwnd)
{
    POINT pt;
    GetCursorPos(&pt);

    HDC hdcWindow = GetDC(hwnd);

    if (!hdcWindow) return RGB(0, 0, 0);

    COLORREF color = GetPixel(hdcWindow, pt.x, pt.y);

    ReleaseDC(NULL, hdcWindow);
    return color;
}

COLORREF GetColorAt(HWND hwnd, int x, int y)
{
    HDC hdcWindow = GetDC(hwnd);
    if (!hdcWindow) return RGB(0, 0, 0);

    COLORREF color = GetPixel(hdcWindow, x, y);
    ReleaseDC(hwnd, hdcWindow);
    return color;
}

bool IsColorSimilar(COLORREF a, COLORREF b, int tolerance)
{
    int r1 = GetRValue(a), g1 = GetGValue(a), b1 = GetBValue(a);
    int r2 = GetRValue(b), g2 = GetGValue(b), b2 = GetBValue(b);

    return abs(r1 - r2) <= tolerance &&
        abs(g1 - g2) <= tolerance &&
        abs(b1 - b2) <= tolerance;
}


bool CheckBattle(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    if (!hdc) return false;

    bool inBattle = (GetPixel(hdc, BattleBlackPixel.x, BattleBlackPixel.y) == RGB(0, 0, 0));

    ReleaseDC(hwnd, hdc);
    return inBattle;
}

bool CheckTarget(HWND hwnd)
{
    int InitTargetx = 7, InitTargety = 98;
    int EndTargetx = 172, EndTargety = 323;

    return CheckColorInArea(hwnd, InitTargetx, InitTargety, EndTargetx - InitTargetx, EndTargety - InitTargety, RGB(255, 0, 0));
}

bool CheckBattleAndTarget(HWND hwnd)
{
    bool retn;
    if (retn = (CheckBattle(hwnd) && !CheckTarget(hwnd)))
    {
        pressKey(hwnd, VK_OEM_6);
    }
    return retn;
}

bool CheckBattleAndTarget2(HWND hwnd)
{
    bool retn;
    if (retn = (CheckBattle(hwnd) && !CheckTarget(hwnd)))
    {
        WInput::SimulateMouseClickAt(hwnd, BattleBlackPixel.x, BattleBlackPixel.y);
    }
    return retn;
}

inline void ForceTarget(HWND hwnd)
{
    pressKey(hwnd, VK_OEM_6);
}

void FullHit(HWND hwnd)
{
    for (int i = VK_F1; i <= VK_F2; ++i)
    {
        pressKey(hwnd, i);
    }
}

bool CheckChatOpen(HWND hwnd)
{
    
    int minR = 140, maxR = 149;
    int minG = 110, maxG = 120;
    int minB = 30, maxB = 33;
    
    HDC hdc = GetDC(NULL); 
    if (hdc)
    {
        COLORREF color = GetPixel(hdc, BotaoDeChatAberto.x, BotaoDeChatAberto.y);
        ReleaseDC(NULL, hdc); 

        int r = GetRValue(color);
        int g = GetGValue(color);
        int b = GetBValue(color);
       
        //std::cout << "Chat pixel RGB: " << r << ", " << g << ", " << b << std::endl;

        return (r >= minR && r <= maxR) &&
            (g >= minG && g <= maxG) &&
            (b >= minB && b <= maxB);
    }
    return false;
}

void AutoDex(HWND hwnd)
{       
    if (!CheckBattle(hwnd))
    {
        return;
    }

    pressKey(hwnd, 0x30);
    Sleep(200);
    WInput::SimulateMouseClickAt(hwnd, BattleBlackPixel.x, BattleBlackPixel.y);
    Sleep(400);
    while (CheckBattle(hwnd) && !(GetAsyncKeyState(VK_ESCAPE)))
    {
        CheckBattleAndTarget2(hwnd);
        Sleep(1000);
        FullHit(hwnd);
    }
}