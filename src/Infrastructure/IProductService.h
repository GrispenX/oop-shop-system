#ifndef SRC_INFRASTRUCTURE_IPRODUCTSERVICE_H_
#define SRC_INFRASTRUCTURE_IPRODUCTSERVICE_H_

#include "Core/Product.h"
#include <optional>
#include <vector>

/**
 * Create a new product with the given name and price.
 * @param name Human-readable product name.
 * @param price Initial price for the product.
 * @returns Integer identifier of the created product.
 */

/**
 * Update the price of an existing product.
 * @param product_id Identifier of the product to update.
 * @param price New price to set.
 */

/**
 * Update the name of an existing product.
 * @param product_id Identifier of the product to update.
 * @param name New human-readable name to set.
 */

/**
 * Associate a discount strategy with a product.
 * @param product_id Identifier of the product to update.
 * @param discount Shared pointer to the discount strategy to apply (may be null to remove a strategy).
 */

/**
 * Retrieve a product by its identifier.
 * @param product_id Identifier of the product to retrieve.
 * @returns `std::optional<Product>` containing the product if found, empty otherwise.
 */

/**
 * Retrieve all products.
 * @returns `std::vector<Product>` containing every product currently available.
 */

/**
 * Get the current stock amount for a product.
 * @param product_id Identifier of the product.
 * @returns Current stock quantity for the specified product.
 */

/**
 * Increase the stock amount for a product by the specified amount.
 * @param product_id Identifier of the product.
 * @param amount Quantity to add to the current stock (must be >= 0).
 */

/**
 * Decrease the stock amount for a product by the specified amount.
 * @param product_id Identifier of the product.
 * @param amount Quantity to remove from the current stock (must be >= 0).
 */
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