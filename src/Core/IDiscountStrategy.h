#ifndef SRC_CORE_IDISCOUNTSTRATEGY_H_
#define SRC_CORE_IDISCOUNTSTRATEGY_H_

#include <string>

class IDiscountStrategy
{
public:
    virtual ~IDiscountStrategy() = default;
    virtual double CalcDiscount(double price, int quantity) = 0;
    virtual std::string GetDescription() = 0;
};

#endif // SRC_CORE_IDISCOUNTSTRATEGY_H_