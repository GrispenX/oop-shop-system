#ifndef SRC_BUSSINESS_RECEIPTSERVICE_H_
#define SRC_BUSSINESS_RECEIPTSERVICE_H_

#include "Core/IProductStorage.h"
#include "Core/IReceiptStorage.h"
#include <memory>

class ReceiptService
{
public:
    ReceiptService(std::shared_ptr<IProductStorage> product_storage, std::shared_ptr<IReceiptStorage> receipt_storage);

    // Product ID - quantity
    int CreateReceipt(std::vector<std::pair<int, int>> items);
    std::vector<Receipt> GetAllReceipts();


private:
    std::shared_ptr<IProductStorage> m_ProductStorage;
    std::shared_ptr<IReceiptStorage> m_ReceiptStorage;
};

#endif // SRC_BUSSINESS_RECEIPTSERVICE_H_