#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../tests/Helpers.h"

#include "Bussiness/ProductService.h"

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

TEST(ProductServiceTest, CreateProductWithPositivePriceReturnsID)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();

    EXPECT_CALL(*product_storage, AddProduct(Product(0, "Test product", 42, nullptr))).Times(1).WillOnce(Return(42));

    ProductService service(product_storage);

    int id = service.CreateProduct("Test product", 42);

    EXPECT_EQ(id, 42);
}

TEST(ProductServiceTest, CreateProductWithNegativePriceThrowsException)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();

    ProductService service(product_storage);

    EXPECT_ANY_THROW(service.CreateProduct("Test product", -42));
}

TEST(ProductServiceTest, SetPriceWithNegativePriceThrowsException)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();

    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(Product(42, "Test product", 69, nullptr)));

    ProductService service(product_storage);

    EXPECT_ANY_THROW(service.SetPrice(42, -1));
}

TEST(ProductServiceTest, GetExistingProductReturnsProduct)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();

    Product product(42, "Test product", 69, nullptr);

    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Return(product));

    ProductService service(product_storage);

    EXPECT_EQ(service.GetProduct(42), product);
}

TEST(ProductServiceTest, GetUnexistingProductReturnsNullopt)
{
    std::shared_ptr<MockProductStorage> product_storage = std::make_shared<MockProductStorage>();

    EXPECT_CALL(*product_storage, GetProduct(42)).Times(1).WillOnce(Throw(std::runtime_error("Product does not exist")));

    ProductService service(product_storage);

    EXPECT_EQ(service.GetProduct(42), std::nullopt);
}