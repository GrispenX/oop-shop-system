#ifndef SRC_BUSSINESS_PRODUCTSERVICE_H_
#define SRC_BUSSINESS_PRODUCTSERVICE_H_

#include "Core/Product.h"
#include "Core/IProductStorage.h"
#include <memory>
#include <vector>
#include <optional>

class ProductService
{
public:
    ProductService(std::shared_ptr<IProductStorage> product_storage);
    
    int CreateProduct(std::string name, double price);
    void SetPrice(int product_id, double price);
    void SetName(int product_id, std::string name);
    std::optional<Product> GetProduct(int product_id);
    std::vector<Product> GetAll();

private:
    std::shared_ptr<IProductStorage> m_ProductStorage;
};

#endif // SRC_BUSSINESS_PRODUCTSERVICE_H_