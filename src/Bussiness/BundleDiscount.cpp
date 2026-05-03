#include "Bussiness/BundleDiscount.h"
#include <format>

BundleDiscount::BundleDiscount(int min_quantity, double percentage) :
    m_MinQuantity(min_quantity),
    m_DiscountPercentage(percentage)
{
    if(percentage <= 0.0 || percentage > 1.0) throw std::runtime_error("Discount percentage should be between 0 and 1");
    if(min_quantity <= 0) throw std::runtime_error("Min quantity should be greater than 0");
}

double BundleDiscount::CalcDiscount(double price, int quantity)
{
    int products_on_discount = quantity / m_MinQuantity;
    return price * m_DiscountPercentage * products_on_discount;
}

std::string BundleDiscount::GetDescription()
{
    return std::format("-{}% for each {}", m_DiscountPercentage * 100, m_MinQuantity);
}