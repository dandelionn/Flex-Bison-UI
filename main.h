#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif




#include <sstream>
#include <windows.h>
#include "WinClass.h"
#include "WinSimpleClass.h"
#include "WinMaker.h"
#include "ResString.h"
#include "resource.h"


#if (_WIN32_IE <= 0x0300)
#define TBSTYLE_FLAT 2048
#define TBSTYLE_LIST 4096
#define TBSTYLE_CUSTOMERASE 8192
#endif


#if defined __MINGW_H
#define _WIN32_IE 0x400
#endif
#include <commctrl.h>



#include <fstream>

BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MDIChildWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ChildWndEnumProc(HWND, LPARAM);

#endif // MAIN_H_INCLUDED
