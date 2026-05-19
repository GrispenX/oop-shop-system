#ifndef SRC_CORE_IINVENTORYSTORAGE_H_
#define SRC_CORE_IINVENTORYSTORAGE_H_

class IInventoryStorage
{
public:
    virtual ~IInventoryStorage() = default;

    virtual int GetStock(int product_id) = 0;
    virtual void SetStock(int product_id, int amount) = 0;
};

#endif // SRC_CORE_IINVENTORYSTORAGE_H_