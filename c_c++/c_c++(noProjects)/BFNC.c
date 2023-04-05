#include<stdio.h>
#include<windows.h>

main()
{
    KEYBDINPUT kbi;
    kbi.wVK = 0x41;
    kbi.wScan = 0;
    kbi.time = 0;
    kbi.dwExtraInfo = (ULONG_PTR) GetMessageExtraInfo();

    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki = kbi;

    SendInput(1, &input, sizeof(INPUT));
}
