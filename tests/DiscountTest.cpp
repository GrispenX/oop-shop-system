#include <gtest/gtest.h>

#include "Bussiness/RegularDiscount.h"
#include "Bussiness/BundleDiscount.h"

TEST(DiscountTest, RegularDiscountAppliesOnEveryProduct)
{
    RegularDiscount discount_strat(0.2);
    
    double discount = discount_strat.CalcDiscount(100, 5);

    EXPECT_EQ(discount, 5 * 20);
}

TEST(DiscountTest, BundleDiscountAppliesOnEveryNProduct)
{
    BundleDiscount discount_strat(3, 0.75);

    double discount = discount_strat.CalcDiscount(100, 14);

    EXPECT_EQ(discount, 4 * 75);
}