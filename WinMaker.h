#ifndef WINMAKER_H_INCLUDED
#define WINMAKER_H_INCLUDED

#include "WinClass.h"

class WinMaker{
    protected:
         HWND         _hwnd;

         DWORD        _exStyle;     ///extended window style
         char const*  _className;    ///class name,just in case _class is not provided
         char const*  _windowName;  ///pointer to window name
         DWORD        _style;       ///window style
         int          _x;           ///horizontal position of window
         int          _y;           ///vertical position of window
         int          _width;       ///windows width
         int          _height;      ///windows height
         HWND         _hWndParent;  ///handle to parent or owner window
         HMENU        _hMenu;       ///handle to menu, or child-window ID
         HINSTANCE    _hInstance;       ///handle to instance, just in case _class is not provided
         void         *_data;       ///pointer to window-creation data

    public:
         WinMaker(char const*,HINSTANCE);
         operator HWND(){return _hwnd;}
         HWND getHWND(){return _hwnd;}
         void AddCaption(char const* caption)
         {
             _windowName = caption;
         }
         void AddSysMenu()    { _style |= WS_SYSMENU;}
         void AddVScrollBar() { _style |= WS_VSCROLL;}
         void AddHScrollBar() { _style |= WS_HSCROLL;}
         void SetPosition(int,int,int,int);
         void AddParent(HWND hWndParent){ _hWndParent = hWndParent;}
         void SetMenu(HMENU hMenu){ _hMenu = hMenu;}
         void SetWndID(int wndID){_hMenu = (HMENU) wndID;}
         void AddStyle(int style = WS_OVERLAPPEDWINDOW | WS_VISIBLE){_style = style;}
         void AddExStyle(int exStyle) { _exStyle = exStyle;}
         void Create();
         void Show(int cmdShow = SW_SHOWNORMAL);
};
#endif // WINMAKER_H_INCLUDED
