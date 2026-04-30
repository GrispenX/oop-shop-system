#ifndef SRC_BUSSINESS_REGULARDISCOUNT_H_
#define SRC_BUSSINESS_REGULARDISCOUNT_H_

#include "Core/IDiscountStrategy.h"

class RegularDiscount : public IDiscountStrategy
{
public:
    RegularDiscount(double percentage);
    double CalcDiscount(double price, int quantity) override;

private:
    double m_DiscountPercentage;
};

#endif // SRC_BUSSINESS_REGULARDISCOUNT_H_