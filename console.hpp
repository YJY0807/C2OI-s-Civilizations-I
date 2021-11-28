#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef BUILD_DLL
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

namespace Console
{
    EXPORT void cInitConsole();                 //Init the Console
    EXPORT void cHideScrollBar();               //Hide the scroll bar
    EXPORT inline void cGotoXY(int x, int y);   //Move the output location
    EXPORT inline void cSetColor(int t, int b); //Set the output color, t is the text color, b is the background color
    EXPORT inline void cSetFont(int x, int y);  //Set the console font
    EXPORT inline void cSetCursor(bool b);      //Show or hide the cursor
    EXPORT inline void cClearScreen();          //Clear the screen
    EXPORT inline int cGetScreenWidth();        //Get the width of console
}

#endif
