#include "Bussiness/BundleDiscount.h"

BundleDiscount::BundleDiscount(int min_quantity, double percentage) :
    m_MinQuantity(min_quantity),
    m_DiscountPercentage(percentage)
{

}

double BundleDiscount::CalcDiscount(double price, int quantity)
{
    int products_on_discount = quantity / m_MinQuantity;
    return price * m_DiscountPercentage * products_on_discount;
}