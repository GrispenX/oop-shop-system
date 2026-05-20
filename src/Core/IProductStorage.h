#ifndef SRC_CORE_IPRODUCTSTORAGE_H_
#define SRC_CORE_IPRODUCTSTORAGE_H_

#include "Core/Product.h"
#include <functional>
#include <vector>
#include <optional>

class IProductStorage
{
public:
    virtual ~IProductStorage() = default;

    virtual int AddProduct(Product product) = 0;
    virtual void UpdateProduct(Product product) = 0;
    virtual std::optional<Product> GetProduct(int id) = 0;
    virtual std::vector<Product> GetProducts(std::function<bool(Product)> predicate) = 0;
    virtual std::vector<Product> GetAllProducts() = 0;
    virtual void RemoveProduct(int id) = 0;
};

#endif // SRC_CORE_IPRODUCTSTORAGE_H_