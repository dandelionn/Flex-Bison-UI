#ifndef RESSTRING_H_INCLUDED
#define RESSTRING_H_INCLUDED
#include <windows.h>

class ResString{
    private:
        char _buf [256];

    public:
         ResString(HINSTANCE hInst, int resId)
        {
              ::LoadString(hInst, resId, _buf, 256);   ///Loads a string resource from the executable file associated with a specified module, copies the string into a buffer, and appends a terminating null character
        }
         char* getString() {return _buf;}
};
#endif // RESSTRING_H_INCLUDED
