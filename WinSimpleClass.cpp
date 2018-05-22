#include "WinSimpleClass.h"

WinSimpleClass::WinSimpleClass(int resID, HINSTANCE hInst): _hInstance(hInst)
{
    ResString resStr(hInst, resID);
    _name = resStr.getString();
}

HWND WinSimpleClass::GetRunningWindow()
{                            ///className, windowName
    HWND hwnd = ::FindWindow(GetName(),     0); ///If the function succeeds, the return value is a handle to the window that has the specified class name and window name.
    if(::IsWindow(hwnd))  ///Determines whether the specified window handle identifies an existing window.
    {
        HWND hwndPopup = ::GetLastActivePopup(hwnd);  ///Determines which pop-up window owned by the specified window was most recently active
        if(::IsWindow(hwndPopup))
            hwnd = hwndPopup;
    }
    else
        hwnd = 0 ;

    return hwnd;
}
