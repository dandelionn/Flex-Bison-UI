#include "WinMaker.h"

WinMaker::WinMaker(char const* className, HINSTANCE hInst):
                                        _hwnd(0),
                                        _exStyle(0),     ///extended window style
                                        _className(className),
                                        _windowName(0),   ///pointer to window name
                                        _style(WS_OVERLAPPED),  ///window style
                                        _x(CW_USEDEFAULT),   ///horizontal position of window
                                        _y(0),   ///vertical position of window
                                        _width(CW_USEDEFAULT), ///window width
                                        _height(0),      ///window height
                                        _hWndParent(0),   ///handle to parent or owner window
                                        _hMenu(0),  ///handle to menu, or child-window identifier
                                        _hInstance(hInst),
                                        _data(0)    ///pointer to window-creation data
                                        {}

void WinMaker::Create()
{
            _hwnd = ::CreateWindowEx(_exStyle,
                                     _className,
                                     _windowName,
                                     _style,
                                     _x,
                                     _y,
                                     _width,
                                     _height,
                                     _hWndParent,
                                     _hMenu,
                                     _hInstance,
                                     _data);
}

void WinMaker::SetPosition(int x, int y, int width, int height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

void WinMaker::Show(int nCmdShow)
{
    ::ShowWindow(_hwnd, nCmdShow);
    ::UpdateWindow(_hwnd);
}
