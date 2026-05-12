#include "Bussiness/RegularCashback.h"

/**
 * @brief Calculates the cashback amount for a given receipt total.
 *
 * @param receipt_total Total value of the receipt in currency units.
 * @return double Cashback amount computed as receipt_total multiplied by the configured percentage.
 */
double RegularCashback::CalcCashback(double receipt_total)
{
    return receipt_total * m_Percentage;
}