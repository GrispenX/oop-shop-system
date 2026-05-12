#include "Infrastructure/JSONInventoryStorage.h"
#include <nlohmann/json.hpp>
#include <fstream>

/**
 * @brief Initialize storage with a JSON file path and load existing inventory if present.
 *
 * The provided filesystem path is stored for later persistence. If a file exists at
 * the path, its JSON content is parsed and the entries in the top-level `"inventory"`
 * array are loaded into the internal stock map as `product_id` → `in_stock`.
 *
 * @param path Filesystem path to the JSON file used for persisting inventory data.
 */
JSONInventoryStorage::JSONInventoryStorage(std::filesystem::path path) :
    m_Path(path)
{
    if(std::filesystem::exists(path))
    {
        std::ifstream file(path);
        nlohmann::json j = nlohmann::json::parse(file);
    
        for(const auto& item : j["inventory"])
        {
            m_Stock.emplace(item["product_id"].get<int>(), item["in_stock"].get<int>());
        }
    
        file.close();
    }
}

/**
 * @brief Persists current inventory to the configured JSON file path on destruction.
 *
 * The destructor serializes the in-memory stock map into a JSON object with an
 * "inventory" array; each entry is an object with `product_id` and `in_stock`
 * fields. The resulting JSON is written to `m_Path` with 4-space indentation.
 */
JSONInventoryStorage::~JSONInventoryStorage()
{
    nlohmann::json j;
    j["inventory"] = nlohmann::json::array();

    for(const auto& [id, stock] : m_Stock)
    {
        j["inventory"].push_back({
            {"product_id", id},
            {"in_stock", stock}
        });
    }

    std::ofstream file(m_Path);
    file << j.dump(4);
    file.close();
}

/**
 * @brief Retrieve the current stock quantity for the specified product.
 *
 * @param product_id Identifier of the product whose stock is requested.
 * @return int Current stock quantity for the product.
 * @throws std::runtime_error If the product_id is not present in storage.
 */
int JSONInventoryStorage::GetStock(int product_id)
{
    auto it = m_Stock.find(product_id);
    if(it == m_Stock.end()) throw std::runtime_error("Product not found");
    return it->second;
}

/**
 * @brief Set the stock quantity for a product in the internal inventory.
 *
 * @param product_id ID of the product to update.
 * @param amount New stock quantity for the product.
 */
void JSONInventoryStorage::SetStock(int product_id, int amount)
{
    m_Stock.insert_or_assign(product_id, amount);
}