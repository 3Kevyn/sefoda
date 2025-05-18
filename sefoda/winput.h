#pragma once
#include <Windows.h>

enum KeyType {
    KEY_SCAN,
    KEY_VIRTUAL
};

namespace WInput {
    extern INPUT kbdUpInput;
    extern INPUT kbdDownInput;
    extern INPUT mouInput;
    extern KeyType keyType;

    inline int AbsW() { return 65536 / GetSystemMetrics(SM_CXSCREEN); }
    inline int AbsH() { return 65536 / GetSystemMetrics(SM_CYSCREEN); }

    void ConfigInput(KeyType kt);

    void SendClick(POINT pt);
    void SendClickRight(POINT pt);
    void SendMouseTo(POINT pt);
    void LockMouse();
    void UnlockMouse();

    inline void SimulateMouseClickAt(HWND hwnd, int x, int y) {
        POINT pt = { x, y };
        ClientToScreen(hwnd, &pt);
        POINT originalPos;
        GetCursorPos(&originalPos);
        SendClick(pt);
        SetCursorPos(originalPos.x, originalPos.y);
    }

    inline void SimulateMouseClickAtR(HWND hwnd, int x, int y) {
        POINT pt = { x, y };
        ClientToScreen(hwnd, &pt);
        POINT originalPos;
        GetCursorPos(&originalPos);
        SendClickRight(pt);
        SetCursorPos(originalPos.x, originalPos.y);
    }
}
