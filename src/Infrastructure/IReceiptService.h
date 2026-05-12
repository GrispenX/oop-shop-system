#ifndef SRC_INFRASTRUCTURE_IRECEIPTSERVICE_H_
#define SRC_INFRASTRUCTURE_IRECEIPTSERVICE_H_

#include "Core/Receipt.h"
#include <optional>

/**
 * Interface for managing receipts lifecycle and retrieval.
 */

/**
 * Start a new receipt and reserve an identifier for further operations.
 * @returns Identifier of the newly created receipt.
 */
 
/**
 * Add a product with the specified quantity to an existing receipt.
 * @param receipt_id Identifier of the receipt to modify.
 * @param product_id Identifier of the product to add.
 * @param quantity Quantity of the product to add (must be positive).
 */
 
/**
 * Associate a customer with an existing receipt.
 * @param receipt_id Identifier of the receipt to modify.
 * @param customer_id Identifier of the customer to associate.
 */
 
/**
 * Finalize an existing receipt, applying the specified cashback usage amount.
 * @param receipt_id Identifier of the receipt to close.
 * @param use_cashback Amount of cashback to apply to the receipt total.
 */
 
/**
 * Cancel an existing receipt.
 * @param receipt_id Identifier of the receipt to cancel.
 */
 
/**
 * Retrieve a receipt by its identifier.
 * @param receipt_id Identifier of the receipt to retrieve.
 * @returns `std::optional<Receipt>` containing the receipt if found, empty otherwise.
 */
 
/**
 * Retrieve all receipts currently stored.
 * @returns A vector containing all stored receipts.
 */
class IReceiptService
{
public:
    virtual ~IReceiptService() = default;

    virtual int StartNewReceipt() = 0;
    virtual void AddItemToReceipt(int receipt_id, int product_id, int quantity) = 0;
    virtual void AddCustomerToReceipt(int receipt_id, int customer_id) = 0;
    virtual void CloseReceipt(int receipt_id, double use_cashback) = 0;
    virtual void CancelReceipt(int receipt_id) = 0;
    virtual std::optional<Receipt> GetReceipt(int receipt_id) = 0;
    virtual std::vector<Receipt> GetAllReceipts() = 0;
};

#endif // SRC_INFRASTRUCTURE_IRECEIPTSERVICE_H_