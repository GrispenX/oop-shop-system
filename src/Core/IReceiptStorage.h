#ifndef SRC_CORE_IRECEIPTSTORAGE_H_
#define SRC_CORE_IRECEIPTSTORAGE_H_

#include "Core/Receipt.h"
#include <functional>
#include <vector>
#include <optional>

class IReceiptStorage
{
public:
    virtual ~IReceiptStorage() = default;

    virtual int AddReceipt(Receipt receipt) = 0;
    virtual void UpdateReceipt(Receipt receipt) = 0;
    virtual std::optional<Receipt> GetReceipt(int id) = 0;
    virtual std::vector<Receipt> GetReceipts(std::function<bool(Receipt)> predicate) = 0;
    virtual std::vector<Receipt> GetAllReceipts() = 0;
    virtual void RemoveReceipt(int id) = 0;
};

#endif // SRC_CORE_IRECEIPTSTORAGE_H_