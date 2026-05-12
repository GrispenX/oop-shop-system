#ifndef SRC_BUSSINESS_RECEIPTSERVICE_H_
#define SRC_BUSSINESS_RECEIPTSERVICE_H_

#include "Infrastructure/IReceiptService.h"
#include "Infrastructure/IProductService.h"
#include "Infrastructure/ICashbackService.h"
#include "Core/IProductStorage.h"
#include "Core/IReceiptStorage.h"

/**
 * Service implementation of IReceiptService that coordinates product and receipt storages
 * with product and cashback business services to manage receipt lifecycle.
 */
 
/**
 * Construct a ReceiptService with required storage and service dependencies.
 * @param product_storage Storage interface used to access and persist product data.
 * @param receipt_storage Storage interface used to access and persist receipt data.
 * @param product_service Business service for product-related operations.
 * @param cashback_service Business service for cashback-related operations.
 */
 
/**
 * Start a new receipt and persist its initial state.
 * @returns The identifier of the newly created receipt.
 */
 
/**
 * Add a product and quantity to an existing receipt.
 * @param receipt_id Identifier of the receipt to modify.
 * @param product_id Identifier of the product to add.
 * @param quantity Quantity of the product to add.
 */
 
/**
 * Associate a customer with an existing receipt.
 * @param receipt_id Identifier of the receipt to modify.
 * @param customer_id Identifier of the customer to attach to the receipt.
 */
 
/**
 * Finalize a receipt and apply payments and cashback as requested.
 * @param receipt_id Identifier of the receipt to close.
 * @param use_cashback Amount of cashback to apply toward the receipt total.
 */
 
/**
 * Cancel an existing receipt, reverting or marking its state as cancelled.
 * @param receipt_id Identifier of the receipt to cancel.
 */
 
/**
 * Retrieve a receipt by its identifier.
 * @param receipt_id Identifier of the receipt to retrieve.
 * @returns An optional containing the receipt if found, or an empty optional if not found.
 */
 
/**
 * Retrieve all receipts.
 * @returns A vector containing all persisted receipts.
 */
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