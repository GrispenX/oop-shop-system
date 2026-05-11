#ifndef SRC_INFRASTRUCTURE_IRECEIPTSERVICE_H_
#define SRC_INFRASTRUCTURE_IRECEIPTSERVICE_H_

#include "Core/Receipt.h"
#include <optional>

class IReceiptService
{
public:
    virtual ~IReceiptService() = default;

    virtual int StartNewReceipt() = 0;
    virtual void AddItemToReceipt(int receipt_id, int product_id, int quantity) = 0;
    virtual void CloseReceipt(int receipt_id) = 0;
    virtual void CancelReceipt(int receipt_id) = 0;
    virtual std::optional<Receipt> GetReceipt(int receipt_id) = 0;
    virtual std::vector<Receipt> GetAllReceipts() = 0;
};

#endif // SRC_INFRASTRUCTURE_IRECEIPTSERVICE_H_