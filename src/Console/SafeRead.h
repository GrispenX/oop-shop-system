#ifndef SRC_CONSOLE_READWITHVALIDATION_H_
#define SRC_CONSOLE_READWITHVALIDATION_H_

#include <iostream>
#include <sstream>
#include <functional>
#include <optional>

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

template<typename T>
bool SafeRead(std::string prompt, T& value)
{
    std::cout << prompt << ": ";
    std::string input;
    std::getline(std::cin, input);

    if constexpr(std::is_same_v<T, std::string>)
    {
        value = input;
        return true;
    }

    std::istringstream ss(input);
    return static_cast<bool>(ss >> value);
}

#endif // SRC_CONSOLE_READWITHVALIDATION_H_