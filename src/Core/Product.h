#ifndef SRC_CORE_PRODUCT_H_
#define SRC_CORE_PRODUCT_H_

#include "Core/IDiscountStrategy.h"
#include <string>
#include <memory>

class Product
{
public:
    Product(int id, std::string name, double price, std::shared_ptr<IDiscountStrategy> discount);

    int GetID();
    std::string GetName();
    double GetPrice();
    std::shared_ptr<IDiscountStrategy> GetDiscount();

private:
    int m_ID;
    std::string m_Name;
    double m_Price;
    std::shared_ptr<IDiscountStrategy> m_Discount;
};

#endif // SRC_CORE_PRODUCT_H_