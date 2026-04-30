#ifndef SRC_CORE_IDISCOUNTSTRATEGY_H_
#define SRC_CORE_IDISCOUNTSTRATEGY_H_

class IDiscountStrategy
{
public:
    virtual ~IDiscountStrategy() = default;
    virtual double CalcDiscount(double price, int quantity) = 0;
};

#endif // SRC_CORE_IDISCOUNTSTRATEGY_H_