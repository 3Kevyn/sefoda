#pragma once
#include <Windows.h>

void VerificarTeclas(HWND hwnd);
bool CheckBattle(HWND hwnd);
bool CheckTarget(HWND hwnd);
bool CheckBattleAndTarget(HWND hwnd);
inline void ForceTarget(HWND hwnd);
void pressKey(HWND hwnd, int key);
void FullHit(HWND hwnd);
bool CheckChatOpen(HWND hwnd);
void AutoDex(HWND hwnd);

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
void DetectarResolucao();
void MonitorWindow();

bool CheckColorInArea(HWND hwnd, int startX, int startY, int width, int height, COLORREF targetColor);
COLORREF GetColorAtMouse(HWND hwnd);
bool IsColorSimilar(COLORREF a, COLORREF b, int tolerance);
