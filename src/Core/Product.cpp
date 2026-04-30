#include "Core/Product.h"
#include <stdexcept>

Product::Product(int id, std::string name, double price, std::shared_ptr<IDiscountStrategy> discount) :
    m_ID(id),
    m_Name(name),
    m_Price(price),
    m_Discount(discount)
{
    if(price < 0) throw std::runtime_error("Price can't be lower than 0");
}

int Product::GetID()
{
    return m_ID;
}

std::string Product::GetName()
{
    return m_Name;
}

double Product::GetPrice()
{
    return m_Price;
}

std::shared_ptr<IDiscountStrategy> Product::GetDiscount()
{
    return m_Discount;
}