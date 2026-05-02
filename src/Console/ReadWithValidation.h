#ifndef SRC_CONSOLE_READWITHVALIDATION_H_
#define SRC_CONSOLE_READWITHVALIDATION_H_

#include <iostream>
#include <sstream>
#include <functional>

template<typename T>
T ReadWithValidation(std::string prompt, std::function<bool(T)> is_valid)
{
    while (true)
    {
        std::cout << prompt << ": ";
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        T input;
        if (iss >> input && is_valid(input)) return input;
    }
}

#endif // SRC_CONSOLE_READWITHVALIDATION_H_