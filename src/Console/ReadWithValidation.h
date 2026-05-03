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
        T input;
        std::cin >> input;
        std::cin.clear();
        std::cin.ignore();
        if(is_valid(input)) return input;
    }
}

#endif // SRC_CONSOLE_READWITHVALIDATION_H_