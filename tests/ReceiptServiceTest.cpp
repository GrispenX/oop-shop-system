#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../tests/Helpers.h"

#include "Infrastructure/ReceiptService.h"

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

class MockProductService : public IProductService
{
public:
    MOCK_METHOD(int, CreateProduct, (std::string name, double price), (override));
    MOCK_METHOD(void, SetPrice, (int product_id, double price), (override));
    MOCK_METHOD(void, SetName, (int product_id, std::string name), (override));
    MOCK_METHOD(void, SetDiscount, (int product_id, std::shared_ptr<IDiscountStrategy> discount), (override));
    MOCK_METHOD(std::optional<Product>, GetProduct, (int product_id), (override));
    MOCK_METHOD(std::vector<Product>, GetAll, (), (override));

    MOCK_METHOD(int, GetStockAmount, (int product_id), (override));
    MOCK_METHOD(void, AddStock, (int product_id, int amount), (override));
    MOCK_METHOD(void, RemoveStock, (int product_id, int amount), (override));
};

TEST(ReceiptServiceTest, StartNewReceiptStoresOpenedReceiptAndReturnsItID)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();
    std::shared_ptr<MockProductService> product_service = std::make_shared<MockProductService>();

    EXPECT_CALL(*receipt_storage, AddReceipt(_)).WillOnce(Return(42));

    ReceiptService service(product_storage, receipt_storage, product_service);

    EXPECT_EQ(service.StartNewReceipt(), 42);
}

TEST(ReceiptServiceTest, AddItemToOpenedReceipt)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();
    std::shared_ptr<MockProductService> product_service = std::make_shared<MockProductService>();

    Product product(69, "Test product", 99.99, nullptr);
    ReceiptItem item(product, 4);
    Receipt before(42, 0, {}, ReceiptStatus::OPENED);
    Receipt after(42, 0, {item}, ReceiptStatus::OPENED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).WillOnce(Return(before));
    EXPECT_CALL(*product_storage, GetProduct(69)).WillOnce(Return(product));
    EXPECT_CALL(*product_service, RemoveStock(69, 4)).Times(1);
    EXPECT_CALL(*receipt_storage, UpdateReceipt(after));

    ReceiptService service(product_storage, receipt_storage, product_service);

    service.AddItemToReceipt(42, 69, 4);
}

TEST(ReceiptServiceTest, AddItemToClosedReceiptThrowsException)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();
    std::shared_ptr<MockProductService> product_service = std::make_shared<MockProductService>();

    Receipt receipt(42, 0, {}, ReceiptStatus::CLOSED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));

    ReceiptService service(product_storage, receipt_storage, product_service);

    EXPECT_ANY_THROW(service.AddItemToReceipt(42, 69, 4));
}

TEST(ReceiptServiceTest, CancelReceiptRemovesItFromStorageAndRestocksProducts)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();
    std::shared_ptr<MockProductService> product_service = std::make_shared<MockProductService>();

    Receipt receipt(42, 0, {ReceiptItem(Product(5, "test product", 100, nullptr), 2)}, ReceiptStatus::OPENED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));
    EXPECT_CALL(*product_service, AddStock(5, 2)).Times(1);
    EXPECT_CALL(*receipt_storage, RemoveReceipt(42)).Times(1);

    ReceiptService service(product_storage, receipt_storage, product_service);

    EXPECT_NO_THROW(service.CancelReceipt(42));
}

TEST(ReceiptServiceTest, CancelClosedReceiptThrowsException)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockReceiptStorage> receipt_storage = std::make_shared<MockReceiptStorage>();
    std::shared_ptr<MockProductService> product_service = std::make_shared<MockProductService>();

    Receipt receipt(42, 0, {}, ReceiptStatus::CLOSED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));

    ReceiptService service(product_storage, receipt_storage, product_service);

    EXPECT_ANY_THROW(service.CancelReceipt(42));
}

