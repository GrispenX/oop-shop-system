#include "Bussiness/RegularDiscount.h"
#include <format>

RegularDiscount::RegularDiscount(double percentage) :
    m_DiscountPercentage(percentage)
{
    if(percentage <= 0.0 || percentage > 1.0) throw std::runtime_error("Discount percentage should be between 0 and 1");
}

double RegularDiscount::CalcDiscount(double price, int quantity)
{
    return price * m_DiscountPercentage * quantity;
}

std::string RegularDiscount::GetDescription()
{
    return std::format("-{}%", m_DiscountPercentage * 100);
}