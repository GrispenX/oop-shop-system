#ifndef SRC_CORE_ICASHBACKSTRATEGY_H_
#define SRC_CORE_ICASHBACKSTRATEGY_H_

class ICashbackStrategy
{
public:
    virtual ~ICashbackStrategy() = default;

    virtual double CalcCashback(double receipt_total) = 0;
};

#endif // SRC_CORE_ICASHBACKSTRATEGY_H_