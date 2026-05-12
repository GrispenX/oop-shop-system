#ifndef SRC_BUSSINESS_REGULARCASHBACK_H_
#define SRC_BUSSINESS_REGULARCASHBACK_H_

#include "Core/ICashbackStrategy.h"

/**
 * Calculate the cashback amount for a receipt using the regular cashback rate.
 *
 * @param receipt_total Total amount of the receipt in monetary units.
 * @returns Cashback amount equal to receipt_total multiplied by the regular rate (0.05).
 */
class RegularCashback : public ICashbackStrategy
{
public:
    double CalcCashback(double receipt_total) override;

private:
    static constexpr double m_Percentage = 0.05;
};

#endif // SRC_BUSSINESS_REGULARCASHBACK_H_