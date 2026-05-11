#ifndef SRC_BUSSINESS_PRODUCTSERVICE_H_
#define SRC_BUSSINESS_PRODUCTSERVICE_H_

#include "Core/Product.h"
#include "Core/IProductStorage.h"
#include "Core/IInventoryStorage.h"
#include <memory>
#include <vector>
#include <optional>

class ProductService
{
public:
    ProductService(std::shared_ptr<IProductStorage> product_storage, std::shared_ptr<IInventoryStorage> inventory_storage);
    
    int CreateProduct(std::string name, double price);
    void SetPrice(int product_id, double price);
    void SetName(int product_id, std::string name);
    void SetDiscount(int product_id, std::shared_ptr<IDiscountStrategy> discount);
    std::optional<Product> GetProduct(int product_id);
    std::vector<Product> GetAll();

    int GetStockAmount(int product_id);
    void AddStock(int product_id, int amount);
    void RemoveStock(int product_id, int amount);

private:
    std::shared_ptr<IProductStorage> m_ProductStorage;
    std::shared_ptr<IInventoryStorage> m_InventoryStorage;
};

#endif // SRC_BUSSINESS_PRODUCTSERVICE_H_