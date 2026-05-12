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

/**
 * @brief Constructs a human-readable description of the bundle discount.
 *
 * @return std::string A string formatted as "-<percentage>% for each <min_quantity>", where `<percentage>` is the discount percentage multiplied by 100 and `<min_quantity>` is the minimum quantity required for the discount.
 */
std::string BundleDiscount::GetDescription()
{
    return std::format("-{}% for each {}", m_DiscountPercentage * 100, m_MinQuantity);
}

/**
 * @brief Minimum quantity required to qualify for the bundle discount.
 *
 * @return int The minimum number of items required for the discount.
 */
int BundleDiscount::GetMinQuantity()
{
    return m_MinQuantity;
}

/**
 * @brief Gets the discount percentage applied per qualifying bundle.
 *
 * @return double The discount percentage as a fraction (value greater than 0 and less than or equal to 1).
 */
double BundleDiscount::GetPercentage()
{
    return m_DiscountPercentage;
}