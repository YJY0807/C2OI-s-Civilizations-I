#define BUILD_DLL

#include <console.hpp>
#include <windows.h>
#include <thread>

using namespace std;

namespace Console
{
    EXPORT void cInitConsole()
    {
        thread tHide(cHideScrollBar);
        tHide.detach();

        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD dMode;
        GetConsoleMode(hStdin, &dMode);
        dMode &= ~ENABLE_QUICK_EDIT_MODE;
        dMode &= ~ENABLE_INSERT_MODE;
        dMode &= ~ENABLE_MOUSE_INPUT;
        SetConsoleMode(hStdin, dMode);

        HWND hCwindow = GetConsoleWindow();
        int cx = GetSystemMetrics(SM_CXSCREEN), cy = GetSystemMetrics(SM_CYSCREEN);
        LONG lWindow = GetWindowLong(hCwindow, GWL_STYLE);
        SetWindowLong(hCwindow, GWL_STYLE, (lWindow | WS_POPUP | WS_MAXIMIZE) & ~WS_THICKFRAME);
        SetWindowPos(hCwindow, HWND_TOP, 0, 0, cx, cy, 0);

        cSetFont(7, 21);

        SetConsoleTitle("C2OI's Civilizations I");
    }

    EXPORT void cHideScrollBar()
    {
        HWND hCwindow = GetConsoleWindow();
        while (1)
        {
            ShowScrollBar(hCwindow, SB_BOTH, FALSE);
        }
    }

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
        wcscpy(cfi.FaceName, L"Microsoft Yahei");
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetCurrentConsoleFontEx(hStdout, FALSE, &cfi);
    }

    EXPORT inline void cSetCursor(bool b)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cci;
        GetConsoleCursorInfo(hStdout, &cci);
        cci.bVisible = FALSE;
        SetConsoleCursorInfo(hStdout, &cci);
    }

    EXPORT inline void cClearScreen()
    {
        COORD cScreen = {0, 0};
        DWORD dCharwritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD dConsolesize;
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(hStdout, &csbi);
        dConsolesize = csbi.dwSize.X * csbi.dwSize.Y;
        FillConsoleOutputCharacter(hStdout, TEXT(' '), dConsolesize, cScreen, &dCharwritten);
        GetConsoleScreenBufferInfo(hStdout, &csbi);
        FillConsoleOutputAttribute(hStdout, csbi.wAttributes, dConsolesize, cScreen, &dCharwritten);
        SetConsoleCursorPosition(hStdout, cScreen);
    }

    EXPORT inline int cGetScreenWidth()
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hStdout, &csbi);
        return csbi.dwSize.X;
    }
}
