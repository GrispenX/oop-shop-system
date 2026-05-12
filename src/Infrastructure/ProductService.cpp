#include "Infrastructure/ProductService.h"

/**
 * @brief Constructs a ProductService and stores the provided storage dependencies.
 *
 * The provided product and inventory storage pointers are retained for all service operations.
 *
 * @param product_storage Shared pointer to the product persistence implementation used for create/read/update/list operations.
 * @param inventory_storage Shared pointer to the inventory storage implementation used for stock management associated with products.
 */
ProductService::ProductService(std::shared_ptr<IProductStorage> product_storage, std::shared_ptr<IInventoryStorage> inventory_storage) :
    m_ProductStorage(product_storage),
    m_InventoryStorage(inventory_storage)
{

}

/**
 * Creates a new product with the given name and price and initializes its inventory stock to zero.
 *
 * @param name Product name.
 * @param price Product price.
 * @return int The id of the newly created product.
 */
int ProductService::CreateProduct(std::string name, double price)
{
    Product product(0, name, price, nullptr);
    int id = m_ProductStorage->AddProduct(product);
    m_InventoryStorage->SetStock(id, 0);
    return id;
}

/**
 * @brief Update the price of an existing product and persist the change.
 *
 * @param product_id Identifier of the product to update.
 * @param price New price to set for the product.
 */
void ProductService::SetPrice(int product_id, double price)
{
    Product product = m_ProductStorage->GetProduct(product_id);
    product.SetPrice(price);
    m_ProductStorage->UpdateProduct(product);
}

void ProductService::SetName(int product_id, std::string name)
{
    Product product = m_ProductStorage->GetProduct(product_id);
    product.SetName(name);
    m_ProductStorage->UpdateProduct(product);
}

std::optional<Product> ProductService::GetProduct(int product_id)
{
    try
    {
        return m_ProductStorage->GetProduct(product_id);
    }
    catch(const std::exception& e)
    {
        return std::nullopt;
    }
}

std::vector<Product> ProductService::GetAll()
{
    return m_ProductStorage->GetAllProducts();
}

/**
 * @brief Set or clear the discount strategy for a product and persist the change.
 *
 * Updates the product identified by |product_id| to use the provided discount
 * strategy; passing a null pointer removes any existing discount. The modified
 * product is persisted via the product storage.
 *
 * @param product_id ID of the product to update.
 * @param discount Shared pointer to the discount strategy to apply, or `nullptr` to remove the discount.
 */
void ProductService::SetDiscount(int product_id, std::shared_ptr<IDiscountStrategy> discount)
{
    Product product = m_ProductStorage->GetProduct(product_id);
    product.SetDiscount(discount);
    m_ProductStorage->UpdateProduct(product);
}

/**
 * @brief Retrieves the current inventory stock for a product.
 *
 * If reading stock from inventory storage fails, the method resets the product's stock to 0 and returns 0.
 *
 * @param product_id Identifier of the product whose stock is requested.
 * @return int Current stock amount for the product; `0` if inventory retrieval failed (after resetting stock to 0).
 */
int ProductService::GetStockAmount(int product_id)
{
    m_ProductStorage->GetProduct(product_id);
    try
    {
        return m_InventoryStorage->GetStock(product_id);
    }
    catch(const std::exception& e)
    {
        m_InventoryStorage->SetStock(product_id, 0);
        return 0;
    }
}

/**
 * @brief Increase the inventory stock for a product by a given positive amount.
 *
 * Ensures the product exists (via the product storage) then increments its stock in the inventory storage.
 *
 * @param product_id Identifier of the product whose stock will be increased.
 * @param amount Number of units to add; must be greater than 0.
 *
 * @throws std::runtime_error if `amount` is less than or equal to 0.
 * @throws std::exception Propagates exceptions thrown by the product or inventory storage operations.
 */
void ProductService::AddStock(int product_id, int amount)
{
    m_ProductStorage->GetProduct(product_id);
    if(amount <= 0) throw std::runtime_error("Amount should be greater than 0");
    try
    {
        int in_stock = m_InventoryStorage->GetStock(product_id);
        m_InventoryStorage->SetStock(product_id, in_stock + amount);
    }
    catch(const std::exception& e)
    {
        m_InventoryStorage->SetStock(product_id, amount);
    }
}

/**
 * @brief Decreases the stored stock for a product by a given amount.
 *
 * Validates the product exists, reads current inventory (resets missing/corrupt inventory to 0),
 * verifies sufficient stock, and updates inventory to reflect the removal.
 *
 * @param product_id Identifier of the product whose stock is to be reduced.
 * @param amount Quantity to remove; must be greater than 0.
 *
 * @throws std::runtime_error if `amount` is less than or equal to 0.
 * @throws std::runtime_error if there is not enough stock to fulfill the removal.
 * @throws std::exception Propagates exceptions from product lookup or inventory update operations.
 */
void ProductService::RemoveStock(int product_id, int amount)
{
    m_ProductStorage->GetProduct(product_id);
    if(amount <= 0) throw std::runtime_error("Amount should be greater than 0");
    int in_stock;
    try
    {
        in_stock = m_InventoryStorage->GetStock(product_id);
    }
    catch(const std::exception& e)
    {
        m_InventoryStorage->SetStock(product_id, 0);
        in_stock = 0;
    }
    if(in_stock - amount < 0) throw std::runtime_error("There is not enough product in stock");
    m_InventoryStorage->SetStock(product_id, in_stock - amount);
}