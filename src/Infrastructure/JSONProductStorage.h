#ifndef SRC_INFRASTRUCTURE_JSONPRODUCTSTORAGE_H_
#define SRC_INFRASTRUCTURE_JSONPRODUCTSTORAGE_H_

#include "Core/IProductStorage.h"
#include <filesystem>

/**
 * JSON-backed implementation of IProductStorage.
 *
 * Stores products in an in-memory map with persistence located at the provided filesystem path.
 */
 
/**
 * Construct a JSONProductStorage that uses the given filesystem path for persistence.
 * @param path Filesystem path where JSON data is read from and written to.
 */
 
/**
 * Destroy the storage, persisting any in-memory changes if applicable.
 */
 
/**
 * Add a new product to storage and assign it a unique integer ID.
 * @param product Product to add; ownership of the ID is transferred to storage.
 * @returns Assigned integer ID for the newly added product.
 */
 
/**
 * Update an existing product in storage identified by the ID within `product`.
 * @param product Product containing an existing ID and the new values to persist.
 */
 
/**
 * Retrieve a product by its integer ID.
 * @param id ID of the product to retrieve.
 * @returns The Product associated with `id`.
 */
 
/**
 * Retrieve all products that satisfy the provided predicate.
 * @param predicate Function used to filter products; called with each Product.
 * @returns Vector of products for which `predicate(product)` returns `true`.
 */
 
/**
 * Retrieve all products currently stored.
 * @returns Vector containing every stored Product.
 */
 
/**
 * Remove the product with the specified ID from storage.
 * @param id ID of the product to remove.
 */
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