#ifndef SRC_INFRASTRUCTURE_IPRODUCTSERVICE_H_
#define SRC_INFRASTRUCTURE_IPRODUCTSERVICE_H_

#include "Core/Product.h"
#include <optional>
#include <vector>

class IProductService
{
public:    
    virtual ~IProductService() = default;

    virtual int CreateProduct(std::string name, double price) = 0;
    virtual void SetPrice(int product_id, double price) = 0;
    virtual void SetName(int product_id, std::string name) = 0;
    virtual void SetDiscount(int product_id, std::shared_ptr<IDiscountStrategy> discount) = 0;
    virtual std::optional<Product> GetProduct(int product_id) = 0;
    virtual std::vector<Product> GetAll() = 0;

    virtual int GetStockAmount(int product_id) = 0;
    virtual void AddStock(int product_id, int amount) = 0;
    virtual void RemoveStock(int product_id, int amount) = 0;
};

#endif // SRC_INFRASTRUCTURE_IPRODUCTSERVICE_H_