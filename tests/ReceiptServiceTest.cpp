#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../tests/Helpers.h"

#include "Bussiness/ReceiptService.h"

using ::testing::Return;
using ::testing::_;

class MockProductStorage : public IProductStorage
{
public:
    MOCK_METHOD(int, AddProduct, (Product product), (override));
    MOCK_METHOD(void, UpdateProduct, (Product product), (override));
    MOCK_METHOD(Product, GetProduct, (int id), (override));
    MOCK_METHOD(std::vector<Product>, GetProducts, (std::function<bool(Product)> predicate), (override));
    MOCK_METHOD(std::vector<Product>, GetAllProducts, (), (override));
    MOCK_METHOD(void, RemoveProduct, (int id), (override));
};

class MockReceiptStorage : public IReceiptStorage
{
public:
    MOCK_METHOD(int, AddReceipt, (Receipt receipt), (override));
    MOCK_METHOD(void, UpdateReceipt, (Receipt receipt), (override));
    MOCK_METHOD(Receipt, GetReceipt, (int id), (override));
    MOCK_METHOD(std::vector<Receipt>, GetReceipts, (std::function<bool(Receipt)> predicate), (override));
    MOCK_METHOD(std::vector<Receipt>, GetAllReceipts, (), (override));
    MOCK_METHOD(void, RemoveReceipt, (int id), (override));
};

TEST(ReceiptServiceTest, StartNewReceiptStoresOpenedReceiptAndReturnsItID)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();

    EXPECT_CALL(*receipt_storage, AddReceipt(_)).WillOnce(Return(42));

    ReceiptService service(product_storage, receipt_storage);

    EXPECT_EQ(service.StartNewReceipt(), 42);
}

TEST(ReceiptServiceTest, AddItemToOpenedReceipt)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();

    Product product(69, "Test product", 99.99, nullptr);
    ReceiptItem item(product, 4);
    Receipt before(42, 0, {}, ReceiptStatus::OPENED);
    Receipt after(42, 0, {item}, ReceiptStatus::OPENED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).WillOnce(Return(before));
    EXPECT_CALL(*product_storage, GetProduct(69)).WillOnce(Return(product));
    EXPECT_CALL(*receipt_storage, UpdateReceipt(after));

    ReceiptService service(product_storage, receipt_storage);

    service.AddItemToReceipt(42, 69, 4);
}

TEST(ReceiptServiceTest, AddItemToClosedReceiptThrowsException)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();

    Receipt receipt(42, 0, {}, ReceiptStatus::CLOSED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));

    ReceiptService service(product_storage, receipt_storage);

    EXPECT_ANY_THROW(service.AddItemToReceipt(42, 69, 4));
}

TEST(ReceiptServiceTest, CancelReceiptRemovesItFromStorage)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();

    Receipt receipt(42, 0, {}, ReceiptStatus::OPENED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));
    EXPECT_CALL(*receipt_storage, RemoveReceipt(42)).Times(1);

    ReceiptService service(product_storage, receipt_storage);

    EXPECT_NO_THROW(service.CancelReceipt(42));
}

TEST(ReceiptServiceTest, CancelClosedReceiptThrowsException)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();

    Receipt receipt(42, 0, {}, ReceiptStatus::CLOSED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));

    ReceiptService service(product_storage, receipt_storage);

    EXPECT_ANY_THROW(service.CancelReceipt(42));
}

