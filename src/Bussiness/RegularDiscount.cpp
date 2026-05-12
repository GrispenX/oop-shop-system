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

/**
 * @brief Produces a human-readable description of the discount as a percentage.
 *
 * The description is formatted with a leading hyphen and a trailing percent sign.
 *
 * @return std::string The discount formatted as "-<value>%" where <value> is the stored discount percentage multiplied by 100.
 */
std::string RegularDiscount::GetDescription()
{
    return std::format("-{}%", m_DiscountPercentage * 100);
}

/**
 * @brief Retrieves the discount percentage stored in the instance.
 *
 * @return double The discount percentage as a fraction greater than 0 and less than or equal to 1.
 */
double RegularDiscount::GetPercentage()
{
    return m_DiscountPercentage;
}