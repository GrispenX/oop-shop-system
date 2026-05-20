#ifndef SRC_INFRASTRUCTURE_JSONINVENTORYSTORAGE_H_
#define SRC_INFRASTRUCTURE_JSONINVENTORYSTORAGE_H_

#include "Core/IInventoryStorage.h"
#include <unordered_map>
#include <filesystem>

class JSONInventoryStorage : public IInventoryStorage
{
public:
    JSONInventoryStorage(std::filesystem::path path);
    ~JSONInventoryStorage();

    std::optional<int> GetStock(int product_id) override;
    void SetStock(int product_id, int amount) override;

private:
    std::filesystem::path m_Path;
    std::unordered_map<int, int> m_Stock;
};

#endif // SRC_INFRASTRUCTURE_JSONINVENTORYSTORAGE_H_