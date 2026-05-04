#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Core/Receipt.h"

using ::testing::Return;

class MockDiscountStrategy : public IDiscountStrategy
{
public:
    MOCK_METHOD(double, CalcDiscount, (double price, int quantity), (override));
    MOCK_METHOD(std::string, GetDescription, (), (override));
};

TEST(ReceiptItemTest, CalcPriceWithoutDiscount)
{
    Product product(0, "Test product", 42, nullptr);
    ReceiptItem ri(product, 5);

    double price = ri.CalcPrice();

    EXPECT_EQ(price, 210);
}

TEST(ReceiptItemTest, CalcPriceWithDiscount)
{
    std::shared_ptr<MockDiscountStrategy> discount_strategy = std::make_shared<MockDiscountStrategy>();
    
    Product product(0, "Test product", 42, discount_strategy);
    ReceiptItem ri(product, 5);

    EXPECT_CALL(*discount_strategy, CalcDiscount(42, 5)).Times(1).WillOnce(Return(10));

    double price = ri.CalcPrice();

    EXPECT_EQ(price, 200);
}

TEST(ReceiptTest, CalcPriceReturnsSumOfItemPrices)
{
    Product p1(1, "Test product 1", 42, nullptr);
    Product p2(2, "Test product 2", 69, nullptr);
    Product p3(3, "Test product 3", 100, nullptr);

    ReceiptItem ri1(p1, 3);
    ReceiptItem ri2(p2, 7);
    ReceiptItem ri3(p3, 4);

    Receipt r(1, 0, {ri1, ri2, ri3}, ReceiptStatus::CLOSED);

    double price = r.CalcTotal();

    EXPECT_EQ(price, 126 + 483 + 400);
}