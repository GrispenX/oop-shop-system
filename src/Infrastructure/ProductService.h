#ifndef SRC_BUSSINESS_PRODUCTSERVICE_H_
#define SRC_BUSSINESS_PRODUCTSERVICE_H_

#include "Infrastructure/IProductService.h"
#include "Core/IProductStorage.h"
#include "Core/IInventoryStorage.h"


class ProductService : public IProductService
{
public:
    ProductService(std::shared_ptr<IProductStorage> product_storage, std::shared_ptr<IInventoryStorage> inventory_storage);
    
    int CreateProduct(std::string name, double price) override;
    void SetPrice(int product_id, double price) override;
    void SetName(int product_id, std::string name) override;
    void SetDiscount(int product_id, std::shared_ptr<IDiscountStrategy> discount) override;
    std::optional<Product> GetProduct(int product_id) override;
    std::vector<Product> GetAll() override;

    int GetStockAmount(int product_id) override;
    void AddStock(int product_id, int amount) override;
    void RemoveStock(int product_id, int amount) override;

private:
    std::shared_ptr<IProductStorage> m_ProductStorage;
    std::shared_ptr<IInventoryStorage> m_InventoryStorage;
};

#endif // SRC_BUSSINESS_PRODUCTSERVICE_H_