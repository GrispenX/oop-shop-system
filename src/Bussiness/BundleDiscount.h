#ifndef SRC_BUSSINESS_BUNDLEDISCOUNT_H_
#define SRC_BUSSINESS_BUNDLEDISCOUNT_H_

#include "Core/IDiscountStrategy.h"

class BundleDiscount : public IDiscountStrategy
{
public:
    BundleDiscount(int min_quantity, double percentage);
    double CalcDiscount(double price, int quantity);
    std::string GetDescription() override;

private:
    int m_MinQuantity;
    double m_DiscountPercentage;
};

#endif // SRC_BUSSINESS_BUNDLEDISCOUNT_H_