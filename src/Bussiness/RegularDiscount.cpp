#include "Bussiness/RegularDiscount.h"

RegularDiscount::RegularDiscount(double percentage) :
    m_DiscountPercentage(percentage)
{

}

double RegularDiscount::CalcDiscount(double price, int quantity)
{
    return price * m_DiscountPercentage * quantity;
}