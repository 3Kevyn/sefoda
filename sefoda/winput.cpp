#include "winput.h"

namespace WInput {
    INPUT kbdUpInput = {};
    INPUT kbdDownInput = {};
    INPUT mouInput = {};
    KeyType keyType = KEY_SCAN;

    void ConfigInput(KeyType kt) {
        keyType = kt;

        kbdUpInput.type = INPUT_KEYBOARD;
        kbdUpInput.ki.dwFlags = kt == KEY_SCAN ? KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE : KEYEVENTF_KEYUP;

        kbdDownInput.type = INPUT_KEYBOARD;
        kbdDownInput.ki.dwFlags = kt == KEY_SCAN ? KEYEVENTF_SCANCODE : 0;
    }

    void SendClick(POINT pt) {
        mouInput.mi.dx = pt.x * AbsW();
        mouInput.mi.dy = pt.y * AbsH();
        mouInput.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
        SendInput(1, &mouInput, sizeof(INPUT));

        mouInput.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &mouInput, sizeof(INPUT));

        mouInput.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &mouInput, sizeof(INPUT));
    }

    void SendClickRight(POINT pt) {
        mouInput.mi.dx = pt.x * AbsW();
        mouInput.mi.dy = pt.y * AbsH();
        mouInput.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
        SendInput(1, &mouInput, sizeof(INPUT));

        mouInput.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        SendInput(1, &mouInput, sizeof(INPUT));

        mouInput.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        SendInput(1, &mouInput, sizeof(INPUT));
    }

    void SendMouseTo(POINT pt) {
        mouInput.mi.dx = pt.x * AbsW();
        mouInput.mi.dy = pt.y * AbsH();
        mouInput.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
        SendInput(1, &mouInput, sizeof(INPUT));
    }

    void LockMouse() {
        RECT rect;
        GetCursorPos((POINT*)&rect);
        rect.right = rect.left + 1;
        rect.bottom = rect.top + 1;
        ClipCursor(&rect);
    }

    void UnlockMouse() {
        ClipCursor(NULL);
    }
}
