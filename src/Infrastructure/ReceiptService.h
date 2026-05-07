#ifndef SRC_BUSSINESS_RECEIPTSERVICE_H_
#define SRC_BUSSINESS_RECEIPTSERVICE_H_

#include "Core/IProductStorage.h"
#include "Core/IReceiptStorage.h"
#include <memory>
#include <optional>

class ReceiptService
{
public:
    ReceiptService(std::shared_ptr<IProductStorage> product_storage, std::shared_ptr<IReceiptStorage> receipt_storage);

    int StartNewReceipt();
    void AddItemToReceipt(int receipt_id, int product_id, int quantity);
    void CloseReceipt(int receipt_id);
    void CancelReceipt(int receipt_id);
    std::optional<Receipt> GetReceipt(int receipt_id);
    std::vector<Receipt> GetAllReceipts();

private:
    std::shared_ptr<IProductStorage> m_ProductStorage;
    std::shared_ptr<IReceiptStorage> m_ReceiptStorage;
};

#endif // SRC_BUSSINESS_RECEIPTSERVICE_H_