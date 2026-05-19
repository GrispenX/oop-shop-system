#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../tests/Helpers.h"

#include "Infrastructure/ProductService.h"

using ::testing::Return;
using ::testing::Throw;

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

class MockInventoryStorage : public IInventoryStorage
{
public:
    MOCK_METHOD(int, GetStock, (int product_id), (override));
    MOCK_METHOD(void, SetStock, (int product_id, int amount), (override));
};

TEST(ProductServiceTest, CreateProductWithPositivePriceReturnsID)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    EXPECT_CALL(*product_storage, AddProduct(Product(0, "Test product", 42, nullptr))).Times(1).WillOnce(Return(42));
    EXPECT_CALL(*inventory_storage, SetStock(42, 0)).Times(1);

    ProductService service(product_storage, inventory_storage);

    int id = service.CreateProduct("Test product", 42);

    EXPECT_EQ(id, 42);
}

TEST(ProductServiceTest, CreateProductWithNegativePriceThrowsException)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    ProductService service(product_storage, inventory_storage);

    EXPECT_ANY_THROW(service.CreateProduct("Test product", -42));
}

TEST(ProductServiceTest, SetPriceWithNegativePriceThrowsException)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(Product(42, "Test product", 69, nullptr)));

    ProductService service(product_storage, inventory_storage);

    EXPECT_ANY_THROW(service.SetPrice(42, -1));
}

TEST(ProductServiceTest, GetExistingProductReturnsProduct)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    Product product(42, "Test product", 69, nullptr);

    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(product));

    ProductService service(product_storage, inventory_storage);

    EXPECT_EQ(service.GetProduct(42), product);
}

TEST(ProductServiceTest, GetUnexistingProductReturnsNullopt)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Throw(std::runtime_error("Product does not exist")));

    ProductService service(product_storage, inventory_storage);

    EXPECT_EQ(service.GetProduct(42), std::nullopt);
}

TEST(ProductServiceTest, GetStockAmountReturnsStock)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    EXPECT_CALL(*product_storage, GetProduct(0)).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(42));

    ProductService service(product_storage, inventory_storage);

    EXPECT_EQ(service.GetStockAmount(0), 42);
}

TEST(ProductServiceTest, GetStockAmountForUnregisteredProductSetsAndReturnsZero)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    EXPECT_CALL(*product_storage, GetProduct(0)).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Throw(std::runtime_error("Product not fiund")));
    EXPECT_CALL(*inventory_storage, SetStock(0, 0)).Times(1);

    ProductService service(product_storage, inventory_storage);

    EXPECT_EQ(service.GetStockAmount(0), 0);
}

TEST(ProductServiceTest, AddStockWithValidIdAndAmount)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    EXPECT_CALL(*product_storage, GetProduct(0)).Times(1).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(20));
    EXPECT_CALL(*inventory_storage, SetStock(0, 25)).Times(1);

    ProductService service(product_storage, inventory_storage);

    service.AddStock(0, 5);
}

TEST(ProductServiceTest, RemoveStockWithValidIdAndAmount)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    EXPECT_CALL(*product_storage, GetProduct(0)).Times(1).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(10));
    EXPECT_CALL(*inventory_storage, SetStock(0, 5)).Times(1);

    ProductService service(product_storage, inventory_storage);

    service.RemoveStock(0, 5);
}

TEST(ProductServiceTest, RemoveStockWithValidIdAndAmountEdge)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    EXPECT_CALL(*product_storage, GetProduct(0)).Times(1).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(5));
    EXPECT_CALL(*inventory_storage, SetStock(0, 0)).Times(1);

    ProductService service(product_storage, inventory_storage);

    service.RemoveStock(0, 5);
}

TEST(ProductServiceTest, RemoveStockForInsufficientProductThrowsException)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();
    std::shared_ptr<MockInventoryStorage> inventory_storage = std::make_shared<MockInventoryStorage>();

    EXPECT_CALL(*product_storage, GetProduct(0)).Times(1).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(4));

    ProductService service(product_storage, inventory_storage);

    EXPECT_ANY_THROW(service.RemoveStock(0, 5));
}

