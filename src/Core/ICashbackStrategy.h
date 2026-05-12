#ifndef SRC_CORE_ICASHBACKSTRATEGY_H_
#define SRC_CORE_ICASHBACKSTRATEGY_H_

/**
 * Interface defining a cashback calculation strategy.
 *
 * Implementations compute a cashback amount based on a receipt total.
 *
 * @note Implementations must be safely destructible through this interface.
 */
 
/**
 * Compute the cashback amount for a given receipt total.
 *
 * @param receipt_total Total value of the receipt used to calculate cashback.
 * @returns Calculated cashback amount as a `double`.
 */
class ICashbackStrategy
{
public:
    virtual ~ICashbackStrategy() = default;

    virtual double CalcCashback(double receipt_total) = 0;
};

#endif // SRC_CORE_ICASHBACKSTRATEGY_H_