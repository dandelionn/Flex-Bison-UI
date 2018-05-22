#include "WinClass.h"
#include "resource.h"
#include <iostream>

void WinClass::SetDefaults ()
{
    ///Provide Reasonable default values
    _class.cbSize = sizeof(WNDCLASSEX);
    _class.style = 0;
    _class.lpszClassName = GetName();
    _class.hInstance = GetInstance();
    _class.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);;
    _class.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);;
    _class.lpszMenuName = 0;
    _class.cbClsExtra = 0;
    _class.cbWndExtra = 0;
    _class.hbrBackground = reinterpret_cast<HBRUSH> (COLOR_WINDOW + 1);
    _class.hCursor = ::LoadCursor(NULL, IDC_ARROW);
}

WinClass::WinClass(char const* className, HINSTANCE hInst, WNDPROC wndProc): WinSimpleClass(className, hInst)
{
    _class.lpfnWndProc = wndProc;
    SetDefaults();
}

WinClass::WinClass(int resId, HINSTANCE hInst, WNDPROC wndProc): WinSimpleClass(resId, hInst)
{
    _class.lpfnWndProc = wndProc;
    SetDefaults();
}

void WinClass::SetMenu(int resId)
{
    _class.lpszMenuName = MAKEINTRESOURCE(resId);
}

void WinClass::SetResIcons(int resId)
{
    _class.hIcon = reinterpret_cast<HICON>(::LoadImage(_class.hInstance,
                                                    MAKEINTRESOURCE(resId),
                                                    IMAGE_ICON,
                                                    ::GetSystemMetrics (SM_CXICON),   //The default width of an icon, in pixels. The LoadIcon function can load only icons with the dimensions that SM_CXICON and SM_CYICON specifies
                                                    ::GetSystemMetrics(SM_CYICON),
                                                     0));
    ///Small icon can be loaded from the same resource
    _class.hIconSm = reinterpret_cast<HICON>(::LoadImage(_class.hInstance,
                                                         MAKEINTRESOURCE(resId),
                                                         IMAGE_ICON,
                                                         ::GetSystemMetrics(SM_CXSMICON), //The recommended width of a small icon, in pixels. Small icons typically appear in window captions and in small icon view
                                                         ::GetSystemMetrics(SM_CYSMICON),
                                                         0));
}

void WinClass::setCursor(int ID)
{
    _class.hCursor = (HCURSOR) ::LoadImage(_class.hInstance ,MAKEINTRESOURCE(ID),IMAGE_ICON, 190,301, LR_LOADTRANSPARENT);
    if(_class.hCursor == NULL)
        MessageBox(NULL,"Image for card not loaded","Cursor not loaded", MB_ICONEXCLAMATION | MB_OK);
}

bool WinClass::Register()
{
    return ::RegisterClassEx(&_class);
}
