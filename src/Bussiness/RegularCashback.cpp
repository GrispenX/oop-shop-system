#include "Bussiness/RegularCashback.h"

double RegularCashback::CalcCashback(double receipt_total)
{
    return receipt_total * m_Percentage;
}