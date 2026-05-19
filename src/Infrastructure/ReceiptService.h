#ifndef SRC_BUSSINESS_RECEIPTSERVICE_H_
#define SRC_BUSSINESS_RECEIPTSERVICE_H_

#include "Infrastructure/IReceiptService.h"
#include "Infrastructure/IProductService.h"
#include "Infrastructure/ICashbackService.h"
#include "Core/IProductStorage.h"
#include "Core/IReceiptStorage.h"

class ReceiptService : public IReceiptService
{
public:
    ReceiptService(
        std::shared_ptr<IProductStorage> product_storage,
        std::shared_ptr<IReceiptStorage> receipt_storage,
        std::shared_ptr<IProductService> product_service,
        std::shared_ptr<ICashbackService> cashback_service
    );

    int StartNewReceipt() override;
    void AddItemToReceipt(int receipt_id, int product_id, int quantity) override;
    void AddCustomerToReceipt(int receipt_id, int customer_id) override;
    void CloseReceipt(int receipt_id, double use_cashback) override;
    void CancelReceipt(int receipt_id) override;
    std::optional<Receipt> GetReceipt(int receipt_id) override;
    std::vector<Receipt> GetAllReceipts() override;

private:
    std::shared_ptr<IProductStorage> m_ProductStorage;
    std::shared_ptr<IReceiptStorage> m_ReceiptStorage;
    std::shared_ptr<IProductService> m_ProductService;
    std::shared_ptr<ICashbackService> m_CashbackService;
};

#endif // SRC_BUSSINESS_RECEIPTSERVICE_H_