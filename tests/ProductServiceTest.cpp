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
    MOCK_METHOD(std::optional<Product>, GetProduct, (int id), (override));
    MOCK_METHOD(std::vector<Product>, GetProducts, (std::function<bool(Product)> predicate), (override));
    MOCK_METHOD(std::vector<Product>, GetAllProducts, (), (override));
    MOCK_METHOD(void, RemoveProduct, (int id), (override));
};

class MockInventoryStorage : public IInventoryStorage
{
public:
    MOCK_METHOD(std::optional<int>, GetStock, (int product_id), (override));
    MOCK_METHOD(void, SetStock, (int product_id, int amount), (override));
};



class ProductServiceTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        product_storage = std::make_shared<MockProductStorage>();
        inventory_storage = std::make_shared<MockInventoryStorage>();
        service = new ProductService(product_storage, inventory_storage);
    }

    void TearDown() override
    {
        delete service;
    }

    ProductService* service;
    std::shared_ptr<MockProductStorage> product_storage;
    std::shared_ptr<MockInventoryStorage> inventory_storage;
};



TEST_F(ProductServiceTest, CreateProductWithPositivePriceReturnsID)
{
    EXPECT_CALL(*product_storage, AddProduct(Product(0, "Test product", 42, nullptr))).Times(1).WillOnce(Return(42));
    EXPECT_CALL(*inventory_storage, SetStock(42, 0)).Times(1);

    int id = service->CreateProduct("Test product", 42);

    EXPECT_EQ(id, 42);
}

TEST_F(ProductServiceTest, CreateProductWithNegativePriceThrowsException)
{
    EXPECT_ANY_THROW(service->CreateProduct("Test product", -42));
}

TEST_F(ProductServiceTest, CreateProductWithZeroPriceThrowsException)
{
    EXPECT_ANY_THROW(service->CreateProduct("Test product", 0));
}

TEST_F(ProductServiceTest, CreateProductWithEmptyNameThrowsException)
{
    EXPECT_ANY_THROW(service->CreateProduct("", 123));
}

TEST_F(ProductServiceTest, SetPozitivePriceForExistingProduct)
{
    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(Product(42, "Test product", 69, nullptr)));
    EXPECT_CALL(*product_storage, UpdateProduct(Product(42, "Test product", 42, nullptr))).Times(1);

    EXPECT_NO_THROW(service->SetPrice(42, 42));
}

TEST_F(ProductServiceTest, SetPriceWithNegativePriceThrowsException)
{
    ON_CALL(*product_storage, GetProduct(42)).WillByDefault(Return(Product(42, "Test product", 69, nullptr)));

    EXPECT_ANY_THROW(service->SetPrice(42, -1));
}

TEST_F(ProductServiceTest, SetPriceWithZeroPriceThrowsException)
{
    ON_CALL(*product_storage, GetProduct(42)).WillByDefault(Return(Product(42, "Test product", 69, nullptr)));

    EXPECT_ANY_THROW(service->SetPrice(42, 0));
}

TEST_F(ProductServiceTest, SetNameForExistingProduct)
{
    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(Product(42, "Test product", 69, nullptr)));

    EXPECT_NO_THROW(service->SetName(42, "New name"));
}

TEST_F(ProductServiceTest, SetEmptyNameForExistingProductThrowsExpection)
{
    EXPECT_ANY_THROW(service->SetName(42, ""));
}

TEST_F(ProductServiceTest, SetNameForUnexistingProductThrowsException)
{
    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->SetName(42, "New name"));
}

TEST_F(ProductServiceTest, GetExistingProductReturnsProduct)
{
    Product product(42, "Test product", 69, nullptr);

    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(product));

    EXPECT_EQ(service->GetProduct(42), product);
}

TEST_F(ProductServiceTest, GetUnexistingProductReturnsNullopt)
{
    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_EQ(service->GetProduct(42), std::nullopt);
}

TEST_F(ProductServiceTest, GetStockAmountReturnsStock)
{
    EXPECT_CALL(*product_storage, GetProduct(0)).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(42));

    EXPECT_EQ(service->GetStockAmount(0), 42);
}

TEST_F(ProductServiceTest, GetStockForUnexistingProductThrowsException)
{
    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->GetStockAmount(42));
}

TEST_F(ProductServiceTest, GetStockAmountForUnregisteredProductSetsAndReturnsZero)
{
    EXPECT_CALL(*product_storage, GetProduct(0)).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(std::nullopt));
    EXPECT_CALL(*inventory_storage, SetStock(0, 0)).Times(1);

    EXPECT_EQ(service->GetStockAmount(0), 0);
}

TEST_F(ProductServiceTest, AddStockWithValidIdAndAmount)
{
    EXPECT_CALL(*product_storage, GetProduct(0)).Times(1).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(20));
    EXPECT_CALL(*inventory_storage, SetStock(0, 25)).Times(1);

    service->AddStock(0, 5);
}

TEST_F(ProductServiceTest, AddStockForUnexistingProductThrowsException)
{
    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->AddStock(42, 5));
}

TEST_F(ProductServiceTest, AddStockWithNegativeAmountThrowsException)
{
    ON_CALL(*product_storage, GetProduct(0)).WillByDefault(Return(Product(0, "test product", 42, nullptr)));
    
    EXPECT_ANY_THROW(service->AddStock(0, -1));
}

TEST_F(ProductServiceTest, RemoveStockWithValidIdAndAmount)
{
    EXPECT_CALL(*product_storage, GetProduct(0)).Times(1).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(10));
    EXPECT_CALL(*inventory_storage, SetStock(0, 5)).Times(1);

    service->RemoveStock(0, 5);
}

TEST_F(ProductServiceTest, RemoveStockWithValidIdAndAmountEdgeCase)
{
    EXPECT_CALL(*product_storage, GetProduct(0)).Times(1).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(5));
    EXPECT_CALL(*inventory_storage, SetStock(0, 0)).Times(1);

    service->RemoveStock(0, 5);
}

TEST_F(ProductServiceTest, RemoveStockForInsufficientProductThrowsException)
{
    EXPECT_CALL(*product_storage, GetProduct(0)).Times(1).WillOnce(Return(Product(0, "test product", 42, nullptr)));
    EXPECT_CALL(*inventory_storage, GetStock(0)).Times(1).WillOnce(Return(4));

    EXPECT_ANY_THROW(service->RemoveStock(0, 5));
}

TEST_F(ProductServiceTest, RemoveStockForUnexistingProductThrowsException)
{
    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->RemoveStock(42, 5));
}