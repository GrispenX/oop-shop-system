#ifndef SRC_INFRASTRUCTURE_JSONINVENTORYSTORAGE_H_
#define SRC_INFRASTRUCTURE_JSONINVENTORYSTORAGE_H_

#include "Core/IInventoryStorage.h"
#include <unordered_map>
#include <filesystem>

/**
 * JSON-backed implementation of IInventoryStorage that persists inventory to a filesystem path.
 *
 * The class maintains an in-memory map of product stock levels and associates it with a
 * filesystem path used for persistence.
 */

/**
 * Construct a JSONInventoryStorage tied to the given filesystem path.
 *
 * @param path Filesystem path to the JSON backing store.
 */
 
/**
 * Destroy the JSONInventoryStorage, performing any necessary cleanup or persistence.
 */
 
/**
 * Retrieve the current stock amount for a product.
 *
 * @param product_id Identifier of the product.
 * @returns `true` if the product exists? No — This must be corrected.
 */

/**
 * Update the stored stock amount for a product.
 *
 * @param product_id Identifier of the product.
 * @param amount New stock amount to set for the product.
 */
class JSONInventoryStorage : public IInventoryStorage
{
public:
    JSONInventoryStorage(std::filesystem::path path);
    ~JSONInventoryStorage();

    int GetStock(int product_id) override;
    void SetStock(int product_id, int amount) override;

private:
    std::filesystem::path m_Path;
    std::unordered_map<int, int> m_Stock;
};

#endif // SRC_INFRASTRUCTURE_JSONINVENTORYSTORAGE_H_