#ifndef SRC_CONSOLE_TERMINALSTYLE_H_
#define SRC_CONSOLE_TERMINALSTYLE_H_

#include <string>

struct Color
{
    int r;
    int g;
    int b;
};

class TerminalStyle
{
public:
    static void Clear();
    static void ResetStyle();
    static void SetForegroundColor(Color color);
    static void SetBackgroundColor(Color color);
    static void SetBold();
    static void SetItalic();
    static void SetUnderline();
    static void SetStrikethrough();

    static void PrintError(std::string msg);
};

#endif // SRC_CONSOLE_TERMINALSTYLE_H_