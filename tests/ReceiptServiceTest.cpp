#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../tests/Helpers.h"
#include "Infrastructure/ReceiptService.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

class MockProductStorage : public IProductStorage
{
public:
    MOCK_METHOD(int, AddProduct, (Product product), (override));
    MOCK_METHOD(void, UpdateProduct, (Product product), (override));
    MOCK_METHOD(std::optional<Product>, GetProduct, (int id), (override));
    MOCK_METHOD(std::vector<Product>, GetProducts, (std::function<bool(Product)> predicate), (override));
    MOCK_METHOD(std::vector<Product>, GetAllProducts, (), (override));
    MOCK_METHOD(void, RemoveProduct, (int id), (override));
};

class MockReceiptStorage : public IReceiptStorage
{
public:
    MOCK_METHOD(int, AddReceipt, (Receipt receipt), (override));
    MOCK_METHOD(void, UpdateReceipt, (Receipt receipt), (override));
    MOCK_METHOD(std::optional<Receipt>, GetReceipt, (int id), (override));
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

class MockCashbackService : public ICashbackService
{
public:
    MOCK_METHOD(int, CreateCustomer, (std::string name, std::string surname, std::shared_ptr<ICashbackStrategy> cashback_strategy), (override));
    MOCK_METHOD(std::optional<Customer>, GetCustomer, (int customer_id), (override));
    MOCK_METHOD(std::vector<Customer>, GetAllCustomers, (), (override));
    MOCK_METHOD(void, SetCustomerName, (int customer_id, std::string name), (override));
    MOCK_METHOD(void, SetCustomerSurname, (int customer_id, std::string surname), (override));
    MOCK_METHOD(void, UseCashback, (int customer_id, double amount), (override));
    MOCK_METHOD(void, AddCashback, (int customer_id, double receipt_total), (override));
};

class MockCashbackStrategy : public ICashbackStrategy
{
public:
    MOCK_METHOD(double, CalcCashback, (double receipt_total), (override));
};



class ReceiptServiceTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        product_storage = std::make_shared<MockProductStorage>();
        receipt_storage = std::make_shared<MockReceiptStorage>();
        product_service = std::make_shared<MockProductService>();
        cashback_service = std::make_shared<MockCashbackService>();
        service = new ReceiptService(product_storage, receipt_storage, product_service, cashback_service);
    }

    void TearDown() override
    {
        delete service;
    }

    std::shared_ptr<MockProductStorage> product_storage;
    std::shared_ptr<MockReceiptStorage> receipt_storage;
    std::shared_ptr<MockProductService> product_service;
    std::shared_ptr<MockCashbackService> cashback_service;
    ReceiptService* service;
};



TEST_F(ReceiptServiceTest, StartNewReceiptStoresOpenedReceiptAndReturnsItID)
{
    EXPECT_CALL(*receipt_storage, AddReceipt(_)).WillOnce(Return(42));

    EXPECT_EQ(service->StartNewReceipt(), 42);
}

TEST_F(ReceiptServiceTest, AddItemToOpenedReceipt)
{
    Product product(69, "Test product", 99.99, nullptr);
    ReceiptItem item(product, 4);
    Receipt before(42, 0, {}, ReceiptStatus::OPENED);
    Receipt after(42, 0, {item}, ReceiptStatus::OPENED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).WillOnce(Return(before));
    EXPECT_CALL(*product_storage, GetProduct(69)).WillOnce(Return(product));
    EXPECT_CALL(*product_service, RemoveStock(69, 4)).Times(1);
    EXPECT_CALL(*receipt_storage, UpdateReceipt(after));

    service->AddItemToReceipt(42, 69, 4);
}

TEST_F(ReceiptServiceTest, AddItemWithUnexistingProductToReceiptThrowsException)
{
    ON_CALL(*receipt_storage, GetReceipt(42)).WillByDefault(Return(Receipt(42, 0, {}, ReceiptStatus::OPENED)));
    EXPECT_CALL(*product_storage, GetProduct(69)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->AddItemToReceipt(42, 69, 5));
}

TEST_F(ReceiptServiceTest, AddItemWithNegativeQuantityThrowsException)
{
    ON_CALL(*receipt_storage, GetReceipt(42)).WillByDefault(Return(Receipt(42, 0, {}, ReceiptStatus::OPENED)));
    ON_CALL(*product_storage, GetProduct(69)).WillByDefault(Return(Product(69, "Test product", 123.50, nullptr)));

    EXPECT_ANY_THROW(service->AddItemToReceipt(42, 69, -1));
}

TEST_F(ReceiptServiceTest, AddItemToUnexistingReceiptThrowsException)
{
    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(std::nullopt));
    ON_CALL(*product_storage, GetProduct(69)).WillByDefault(Return(Product(69, "Test product", 123.50, nullptr)));

    EXPECT_ANY_THROW(service->AddItemToReceipt(42, 69, 5));
}

TEST_F(ReceiptServiceTest, AddItemToClosedReceiptThrowsException)
{
    Receipt receipt(42, 0, {}, ReceiptStatus::CLOSED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));

    EXPECT_ANY_THROW(service->AddItemToReceipt(42, 69, 4));
}

TEST_F(ReceiptServiceTest, CancelReceiptRemovesItFromStorageAndRestocksProducts)
{
    Receipt receipt(42, 0, {ReceiptItem(Product(5, "test product", 100, nullptr), 2)}, ReceiptStatus::OPENED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));
    EXPECT_CALL(*product_service, AddStock(5, 2)).Times(1);
    EXPECT_CALL(*receipt_storage, RemoveReceipt(42)).Times(1);

    EXPECT_NO_THROW(service->CancelReceipt(42));
}

TEST_F(ReceiptServiceTest, CancelClosedReceiptThrowsException)
{
    Receipt receipt(42, 0, {}, ReceiptStatus::CLOSED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));

    EXPECT_ANY_THROW(service->CancelReceipt(42));
}

TEST_F(ReceiptServiceTest, CancelUnexistingReceiptThrowsException)
{
    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->CancelReceipt(42));
}

TEST_F(ReceiptServiceTest, CloseReceiptWithoutCashbackChangesStatus)
{
    Receipt before(42, 0, {ReceiptItem(Product(69, "Test product", 123, nullptr), 5)}, ReceiptStatus::OPENED);
    Receipt after(42, 0, {ReceiptItem(Product(69, "Test product", 123, nullptr), 5)}, ReceiptStatus::CLOSED);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(before));
    EXPECT_CALL(*receipt_storage, UpdateReceipt(after)).Times(1);

    EXPECT_NO_THROW(service->CloseReceipt(42, 0));
}

TEST_F(ReceiptServiceTest, CloseUnexistingReceiptThrowsException)
{
    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->CloseReceipt(42, 0));
}

TEST_F(ReceiptServiceTest, CloseReceiptUsingCashback)
{
    Receipt before(42, 0, {ReceiptItem(Product(69, "Test product", 100, nullptr), 5)}, ReceiptStatus::OPENED, 1);
    Receipt after(42, 0, {ReceiptItem(Product(69, "Test product", 100, nullptr), 5)}, ReceiptStatus::CLOSED, 1, 5);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(before));
    EXPECT_CALL(*cashback_service, UseCashback(1, 5)).Times(1);
    EXPECT_CALL(*cashback_service, AddCashback(1, 495));
    EXPECT_CALL(*receipt_storage, UpdateReceipt(after)).Times(1);

    EXPECT_NO_THROW(service->CloseReceipt(42, 5));
}

TEST_F(ReceiptServiceTest, CloseReceiptWithoutCustomerUsingCashbackThrowsException)
{
    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(Receipt(42, 0, {}, ReceiptStatus::OPENED)));
    
    EXPECT_ANY_THROW(service->CloseReceipt(42, 5));
}

TEST_F(ReceiptServiceTest, CloseReceiptUsingMoreCashbackThanTotalThrowsException)
{
    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(Receipt(42, 0, {ReceiptItem(Product(69, "Test product", 100, nullptr), 1)}, ReceiptStatus::OPENED, 1, 0)));

    EXPECT_ANY_THROW(service->CloseReceipt(42, 200));
}

TEST_F(ReceiptServiceTest, CloseReceiptUsingCashbackWithInsuffitientBalanceTHrowsException)
{
    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(Receipt(42, 0, {ReceiptItem(Product(69, "Test product", 100, nullptr), 1)}, ReceiptStatus::OPENED, 1, 0)));
    EXPECT_CALL(*cashback_service, UseCashback(1, 10)).Times(1).WillOnce(Throw(std::runtime_error("There is not enough cashback on balance")));

    EXPECT_ANY_THROW(service->CloseReceipt(42, 10));
}

TEST_F(ReceiptServiceTest, AddValidCustomerToValidOpenedReceipt)
{
    Receipt receipt(42, 0, {}, ReceiptStatus::OPENED);
    Customer customer(69, "test", "customer", 123, std::make_shared<MockCashbackStrategy>());
    Receipt receipt_after(42, 0, {}, ReceiptStatus::OPENED, 123, 0);

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));
    EXPECT_CALL(*cashback_service, GetCustomer(69)).Times(1).WillOnce(Return(customer));
    EXPECT_CALL(*receipt_storage, UpdateReceipt(receipt_after)).Times(1);

    service->AddCustomerToReceipt(42, 69);
}

TEST_F(ReceiptServiceTest, AddValidCustomerToUnexistingReceiptThrowsException)
{
    Customer customer(69, "test", "customer", 123, std::make_shared<MockCashbackStrategy>());

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Throw(std::runtime_error("Receipt not found")));
    ON_CALL(*cashback_service, GetCustomer(69)).WillByDefault(Return(customer));

    EXPECT_ANY_THROW(service->AddCustomerToReceipt(42, 69));
}

TEST_F(ReceiptServiceTest, AddUnexistingCustomerToValidReceiptThrowsException)
{
    Receipt receipt(42, 0, {}, ReceiptStatus::OPENED);

    ON_CALL(*receipt_storage, GetReceipt(42)).WillByDefault(Return(receipt));
    EXPECT_CALL(*cashback_service, GetCustomer(69)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->AddCustomerToReceipt(42, 69));
}

TEST_F(ReceiptServiceTest, AddValidCustomerToClosedReceiptThrowsException)
{
    Receipt receipt(42, 0, {}, ReceiptStatus::CLOSED);
    Customer customer(69, "test", "customer", 123, std::make_shared<MockCashbackStrategy>());

    EXPECT_CALL(*receipt_storage, GetReceipt(42)).Times(1).WillOnce(Return(receipt));
    ON_CALL(*cashback_service, GetCustomer(69)).WillByDefault(Return(customer));

    EXPECT_ANY_THROW(service->AddCustomerToReceipt(42, 69));
}