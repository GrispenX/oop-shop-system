#ifndef SRC_INFRASTRUCTURE_JSONPRODUCTSTORAGE_H_
#define SRC_INFRASTRUCTURE_JSONPRODUCTSTORAGE_H_

#include "Core/IProductStorage.h"
#include <filesystem>

class JSONProductStorage : public IProductStorage
{
public:
    JSONProductStorage(std::filesystem::path path);
    ~JSONProductStorage();

    int AddProduct(Product product) override;
    void UpdateProduct(Product product) override;
    Product GetProduct(int id) override;
    std::vector<Product> GetProducts(std::function<bool(Product)> predicate) override;
    std::vector<Product> GetAllProducts() override;
    void RemoveProduct(int id) override;

private:
    std::filesystem::path m_Path;
    std::unordered_map<int, Product> m_Products;
    int m_NextID;
};

#endif // SRC_INFRASTRUCTURE_JSONPRODUCTSTORAGE_H_