#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Helpers.h"

#include "Infrastructure/JSONCustomerSerializer.h"
#include "Infrastructure/JSONProductSerializer.h"
#include "Infrastructure/JSONReceiptSerializer.h"
#include "Bussiness/RegularCashback.h"

TEST(JSONSerializationTest, CustomerSerialization)
{
    Customer customer(42, "Till", "Lindemann", 100.99, std::make_shared<RegularCashback>());
    nlohmann::json customer_json = {
        {"id", 42},
        {"name", "Till"},
        {"surname", "Lindemann"},
        {"cashback_balance", 100.99},
        {"cashback_strategy", {
            {"type", "regular"}
        }}
    };
    JSONCustomerSerializer serializer;
    EXPECT_EQ(serializer.Serialize(customer), customer_json);
}

TEST(JSONSerializationTest, CustomerDeserialization)
{
    nlohmann::json customer_json = {
        {"id", 42},
        {"name", "Till"},
        {"surname", "Lindemann"},
        {"cashback_balance", 100.99},
        {"cashback_strategy", {
            {"type", "regular"}
        }}
    };
    JSONCustomerSerializer serializer;
    Customer customer = serializer.Deserialize(customer_json);
    EXPECT_EQ(customer.GetID(), 42);
    EXPECT_EQ(customer.GetName(), "Till");
    EXPECT_EQ(customer.GetSurname(), "Lindemann");
    EXPECT_EQ(customer.GetBalance(), 100.99);
}

TEST(JSONSerializationTest, ProductSerialization)
{
    Product product(42, "Beer", 50.5, nullptr);
    nlohmann::json product_json = {
        {"id", 42},
        {"name", "Beer"},
        {"price", 50.5},
        {"discount", {
            {"type", "none"}
        }}
    };
    JSONProductSerializer serializer;
    EXPECT_EQ(serializer.Serialize(product), product_json);
}

TEST(JSONSerializationTest, ProductDeserialization)
{
    nlohmann::json product_json = {
        {"id", 42},
        {"name", "Beer"},
        {"price", 50.5},
        {"discount", {
            {"type", "none"}
        }}
    };
    JSONProductSerializer serializer;
    Product product = serializer.Deserialize(product_json);
    EXPECT_EQ(product.GetID(), 42);
    EXPECT_EQ(product.GetName(), "Beer");
    EXPECT_EQ(product.GetPrice(), 50.5);
    EXPECT_EQ(product.GetDiscount(), nullptr);
}

TEST(JSONSerializationTest, ReceiptSerialization)
{
    Product product1(42, "Beer", 50, nullptr), product2(69, "Wine", 100, nullptr);
    ReceiptItem item1(product1, 5), item2(product2, 3);
    Receipt receipt(5, 123, {item1, item2}, ReceiptStatus::CLOSED, 20, 10);
    nlohmann::json receipt_json = {
        {"id", 5},
        {"timestamp", 123},
        {"status", ReceiptStatus::CLOSED},
        {"customer_id", 20},
        {"used_cashback", 10},
        {"items", nlohmann::json::array(
            {{
                {"product", {
                    {"id", 42},
                    {"name", "Beer"},
                    {"price", 50},
                    {"discount", {
                        {"type", "none"}
                    }}
                }},
                {"quantity", 5}
            },
            {
                {"product", {
                    {"id", 69},
                    {"name", "Wine"},
                    {"price", 100},
                    {"discount", {
                        {"type", "none"}
                    }}
                }},
                {"quantity", 3}
            }}
        )}
    };
    JSONReceiptSerializer serializer;
    EXPECT_EQ(serializer.Serialize(receipt), receipt_json);
}

TEST(JSONSerializationTest, ReceiptDeserialization)
{
    nlohmann::json receipt_json = {
        {"id", 5},
        {"timestamp", 123},
        {"status", ReceiptStatus::CLOSED},
        {"customer_id", 20},
        {"used_cashback", 10},
        {"items", nlohmann::json::array(
            {{
                {"product", {
                    {"id", 42},
                    {"name", "Beer"},
                    {"price", 50},
                    {"discount", {
                        {"type", "none"}
                    }}
                }},
                {"quantity", 5}
            },
            {
                {"product", {
                    {"id", 69},
                    {"name", "Wine"},
                    {"price", 100},
                    {"discount", {
                        {"type", "none"}
                    }}
                }},
                {"quantity", 3}
            }}
        )}
    };
    JSONReceiptSerializer serializer;
    Receipt receipt = serializer.Deserialize(receipt_json);
    EXPECT_EQ(receipt.GetID(), 5);
    EXPECT_EQ(receipt.GetTimestamp(), 123);
    EXPECT_EQ(receipt.GetStatus(), ReceiptStatus::CLOSED);
    EXPECT_EQ(receipt.GetCustomerID().value(), 20);
    EXPECT_EQ(receipt.GetUsedCashback(), 10);
    EXPECT_EQ(receipt.CalcTotal(), 540);
}