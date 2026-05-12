#ifndef SRC_BUSSINESS_PRODUCTSERVICE_H_
#define SRC_BUSSINESS_PRODUCTSERVICE_H_

#include "Infrastructure/IProductService.h"
#include "Core/IProductStorage.h"
#include "Core/IInventoryStorage.h"


/**
 * Construct a ProductService with the provided product and inventory storage implementations.
 * @param product_storage Shared pointer to the product storage implementation used for product data.
 * @param inventory_storage Shared pointer to the inventory storage implementation used for stock data.
 */

/**
 * Create a new product with the given name and price and persist it.
 * @param name The product's name.
 * @param price The product's price.
 * @returns The identifier of the newly created product.
 */

/**
 * Update the stored price for the specified product.
 * @param product_id Identifier of the product to update.
 * @param price New price to set for the product.
 */

/**
 * Update the stored name for the specified product.
 * @param product_id Identifier of the product to update.
 * @param name New name to set for the product.
 */

/**
 * Associate a discount strategy with the specified product.
 * @param product_id Identifier of the product to update.
 * @param discount Shared pointer to the discount strategy to apply to the product.
 */

/**
 * Retrieve a product by its identifier.
 * @param product_id Identifier of the product to retrieve.
 * @returns An optional containing the product if found, or an empty optional if not present.
 */

/**
 * Retrieve all products.
 * @returns A vector containing all stored products.
 */

/**
 * Get the current stock amount for a specific product.
 * @param product_id Identifier of the product whose stock amount to retrieve.
 * @returns The current stock amount for the product.
 */

/**
 * Increase the stock amount for a specific product by the given amount.
 * @param product_id Identifier of the product to add stock to.
 * @param amount Number of units to add to the product's stock.
 */

/**
 * Decrease the stock amount for a specific product by the given amount.
 * @param product_id Identifier of the product to remove stock from.
 * @param amount Number of units to remove from the product's stock.
 */
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