#ifndef SRC_BUSSINESS_REGULARDISCOUNT_H_
#define SRC_BUSSINESS_REGULARDISCOUNT_H_

#include "Core/IDiscountStrategy.h"

/**
 * Regular discount strategy that applies a fixed percentage discount to items.
 */
 
/**
 * Initialize the regular discount strategy with a percentage.
 * @param percentage Discount percentage expressed as a fraction of 1.0 (e.g., 0.20 for 20%).
 */
 
/**
 * Calculate the total discount amount for a line item.
 * @param price Unit price of the item.
 * @param quantity Number of items.
 * @returns The monetary discount amount to subtract from (price * quantity).
 */
 
/**
 * Human-readable description of this discount strategy.
 * @returns A string describing the discount and its percentage.
 */
 
/**
 * Get the configured discount percentage.
 * @returns The discount percentage as a fraction of 1.0 (e.g., 0.20 for 20%).
 */
class RegularDiscount : public IDiscountStrategy
{
public:
    RegularDiscount(double percentage);
    double CalcDiscount(double price, int quantity) override;
    std::string GetDescription() override;
    double GetPercentage();

private:
    double m_DiscountPercentage;
};

#endif // SRC_BUSSINESS_REGULARDISCOUNT_H_