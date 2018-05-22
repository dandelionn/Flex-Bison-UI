#ifndef WINSIMPLECLASS_H_INCLUDED
#define WINSIMPLECLASS_H_INCLUDED

#include <string>
#include <windows.h>
#include "ResString.h"

class WinSimpleClass{
protected:
    std::string _name;
    HINSTANCE _hInstance;

public:
    WinSimpleClass(char const* name, HINSTANCE hInst) : _name(name), _hInstance(hInst) {};
    WinSimpleClass(int resId, HINSTANCE hInst);

    char const* GetName() const { return _name.c_str(); }
    HINSTANCE GetInstance() const { return _hInstance; }
    HWND GetRunningWindow();
};
#endif // WINSIMPLECLASS_H_INCLUDED
