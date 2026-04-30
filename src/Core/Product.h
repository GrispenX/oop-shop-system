#ifndef SRC_CORE_PRODUCT_H_
#define SRC_CORE_PRODUCT_H_

#include <string>

class Product
{
public:
    Product(std::string name, double price);

    std::string GetName();
    double GetPrice();

private:
    std::string m_Name;
    double m_Price;
};

#endif // SRC_CORE_PRODUCT_H_