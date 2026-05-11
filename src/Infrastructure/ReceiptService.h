#ifndef SRC_BUSSINESS_RECEIPTSERVICE_H_
#define SRC_BUSSINESS_RECEIPTSERVICE_H_

#include "Infrastructure/IReceiptService.h"
#include "Core/IProductStorage.h"
#include "Core/IReceiptStorage.h"

class ReceiptService : public IReceiptService
{
public:
    ReceiptService(std::shared_ptr<IProductStorage> product_storage, std::shared_ptr<IReceiptStorage> receipt_storage);

    int StartNewReceipt() override;
    void AddItemToReceipt(int receipt_id, int product_id, int quantity) override;
    void CloseReceipt(int receipt_id) override;
    void CancelReceipt(int receipt_id) override;
    std::optional<Receipt> GetReceipt(int receipt_id) override;
    std::vector<Receipt> GetAllReceipts() override;

private:
    std::shared_ptr<IProductStorage> m_ProductStorage;
    std::shared_ptr<IReceiptStorage> m_ReceiptStorage;
};

#endif // SRC_BUSSINESS_RECEIPTSERVICE_H_