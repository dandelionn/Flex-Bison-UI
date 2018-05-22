#ifndef WINCLASS_H_INCLUDED
#define WINCLASS_H_INCLUDED

#include "WinSimpleClass.h"
#include "windows.h"

class WinClass: public WinSimpleClass
{
protected:
    void SetDefaults();
    WNDCLASSEX _class;

public:
    WinClass (char const* className, HINSTANCE hInst, WNDPROC wndProc);
    WinClass (int resID, HINSTANCE hInst, WNDPROC wndProc);
    void SetBgSysColor( int sysColor)
    {
        _class.hbrBackground = reinterpret_cast<HBRUSH> (sysColor + 1);
    }
    void SetMenu(int resId);
    void SetResIcons(int resId);
    void setCursor(int ID);
    bool Register();
};
#endif // WINCLASS_H_INCLUDED
