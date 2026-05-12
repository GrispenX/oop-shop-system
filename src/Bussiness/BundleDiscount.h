#ifndef SRC_BUSSINESS_BUNDLEDISCOUNT_H_
#define SRC_BUSSINESS_BUNDLEDISCOUNT_H_

#include "Core/IDiscountStrategy.h"

/**
 * Represents a bundle discount strategy that applies a percentage discount when
 * an order's quantity meets or exceeds a minimum threshold.
 */
 
/**
 * Initialize a bundle discount with a minimum qualifying quantity and a discount percentage.
 * @param min_quantity Minimum quantity required for the discount to apply (inclusive).
 * @param percentage Discount percentage expressed as a decimal or percent value consistent with callers' expectations.
 */
 
/**
 * Calculate the discount amount for an item price given a purchased quantity.
 * @param price Unit price used to compute the discount amount.
 * @param quantity Quantity purchased to evaluate qualification and scale the discount.
 * @returns The monetary discount to apply for the given price and quantity.
 */
 
/**
 * Get a human-readable description of this discount rule.
 * @returns A string describing the bundle discount (e.g., qualification and percentage).
 */
 
/**
 * Get the minimum quantity required to qualify for the bundle discount.
 * @returns The minimum qualifying quantity.
 */
 
/**
 * Get the discount percentage applied when the minimum quantity is met.
 * @returns The discount percentage.
 */
class BundleDiscount : public IDiscountStrategy
{
public:
    BundleDiscount(int min_quantity, double percentage);
    double CalcDiscount(double price, int quantity);
    std::string GetDescription() override;
    int GetMinQuantity();
    double GetPercentage();

private:
    int m_MinQuantity;
    double m_DiscountPercentage;
};

#endif // SRC_BUSSINESS_BUNDLEDISCOUNT_H_