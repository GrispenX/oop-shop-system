#include "Console/TerminalStyle.h"
#include <iostream>
#include <format>

void TerminalStyle::Clear()
{
    std::cout << "\033[2J";
}

void TerminalStyle::ResetStyle()
{
    std::cout << "\033[0m";
}

void TerminalStyle::SetForegroundColor(Color color)
{
    std::cout << std::format("\033[38;2;{};{};{}m", color.r, color.g, color.b);
}

void TerminalStyle::SetBackgroundColor(Color color)
{
    std::cout << std::format("\033[48;2;{};{};{}m", color.r, color.g, color.b);
}

void TerminalStyle::SetBold()
{
    std::cout << "\033[1m";
}

void TerminalStyle::SetItalic()
{
    std::cout << "\033[3m";
}

void TerminalStyle::SetUnderline()
{
    std::cout << "\033[4m";
}

void TerminalStyle::SetStrikethrough()
{
    std::cout << "\033[9m";
}

void TerminalStyle::PrintError(std::string msg)
{
    SetForegroundColor(Color{255, 0, 0});
    SetBold();
    std::cout << msg;
    ResetStyle();
    std::cout << "\n";
}