#include "Infrastructure/JSONInventoryStorage.h"
#include <nlohmann/json.hpp>
#include <fstream>

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

std::optional<int> JSONInventoryStorage::GetStock(int product_id)
{
    auto it = m_Stock.find(product_id);
    if(it == m_Stock.end()) return std::nullopt;
    return it->second;
}

void JSONInventoryStorage::SetStock(int product_id, int amount)
{
    m_Stock.insert_or_assign(product_id, amount);
}