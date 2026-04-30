#include "Core/Product.h"
#include <stdexcept>

Product::Product(std::string name, double price) :
    m_Name(name),
    m_Price(price)
{
    if(price < 0) throw std::runtime_error("Price can't be lower than 0");
}

std::string Product::GetName()
{
    return m_Name;
}

double Product::GetPrice()
{
    return m_Price;
}