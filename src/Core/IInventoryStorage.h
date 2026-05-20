#ifndef SRC_CORE_IINVENTORYSTORAGE_H_
#define SRC_CORE_IINVENTORYSTORAGE_H_

#include <optional>

class IInventoryStorage
{
public:
    virtual ~IInventoryStorage() = default;

    virtual std::optional<int> GetStock(int product_id) = 0;
    virtual void SetStock(int product_id, int amount) = 0;
};

#endif // SRC_CORE_IINVENTORYSTORAGE_H_