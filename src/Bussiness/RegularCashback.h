#ifndef SRC_BUSSINESS_REGULARCASHBACK_H_
#define SRC_BUSSINESS_REGULARCASHBACK_H_

#include "Core/ICashbackStrategy.h"

class RegularCashback : public ICashbackStrategy
{
public:
    double CalcCashback(double receipt_total) override;

private:
    static constexpr double m_Percentage = 0.05;
};

#endif // SRC_BUSSINESS_REGULARCASHBACK_H_