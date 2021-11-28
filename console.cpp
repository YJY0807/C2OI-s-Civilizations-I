#define BUILD_DLL

#include <console.hpp>
#include <windows.h>

using namespace std;

namespace Console
{
    EXPORT inline void cGotoXY(int x, int y)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD c;
        c.X = y;
        c.Y = x;
        SetConsoleCursorPosition(hStdout, c);
    }
    EXPORT inline void cSetColor(int t, int b)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout, t + b * 16);
    }
    EXPORT inline void cSetFont(int x, int y)
    {
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof cfi;
        cfi.nFont = 0;
        cfi.dwFontSize.X = x;
        cfi.dwFontSize.Y = y;
        cfi.FontWeight = FW_THIN;
        cfi.FontFamily = FF_DONTCARE;
        wcscpy(cfi.FaceName, L"Raster");
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    }
}