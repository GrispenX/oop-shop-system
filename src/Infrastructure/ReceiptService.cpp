#include "Infrastructure/ReceiptService.h"

/**
 * @brief Initializes the ReceiptService with its required storage and service dependencies.
 *
 * @param product_storage Storage backend for product data used to load product information.
 * @param receipt_storage Storage backend for receipts used to persist and retrieve receipts.
 * @param product_service Service used to modify product inventory (add/remove stock).
 * @param cashback_service Service used to query customers and manage cashback balances.
 */
ReceiptService::ReceiptService(
    std::shared_ptr<IProductStorage> product_storage,
    std::shared_ptr<IReceiptStorage> receipt_storage,
    std::shared_ptr<IProductService> product_service,
    std::shared_ptr<ICashbackService> cashback_service
) :
    m_ProductStorage(product_storage),
    m_ReceiptStorage(receipt_storage),
    m_ProductService(product_service),
    m_CashbackService(cashback_service)
{

}

int ReceiptService::StartNewReceipt()
{
    Receipt receipt(0, time(nullptr), {}, ReceiptStatus::OPENED);
    return m_ReceiptStorage->AddReceipt(receipt);
}

/**
 * @brief Adds a product item to an opened receipt and decrements inventory.
 *
 * Adds the specified product and quantity to the receipt identified by `receipt_id`,
 * removes that quantity from product stock, and persists the updated receipt.
 *
 * @param receipt_id ID of the receipt to modify.
 * @param product_id ID of the product to add.
 * @param quantity Number of units to add; must be greater than zero.
 *
 * @throws std::runtime_error if the receipt is closed.
 * @throws std::exception Propagates exceptions from storage or service calls (for example: product not found or insufficient stock).
 */
void ReceiptService::AddItemToReceipt(int receipt_id, int product_id, int quantity)
{
    Receipt receipt = m_ReceiptStorage->GetReceipt(receipt_id);
    if(receipt.GetStatus() == ReceiptStatus::CLOSED) throw std::runtime_error("Can't add item to closed receipt");

    Product product = m_ProductStorage->GetProduct(product_id);
    m_ProductService->RemoveStock(product_id, quantity);
    ReceiptItem item(product, quantity);
    receipt.AddItem(item);
    m_ReceiptStorage->UpdateReceipt(receipt);
}

/**
 * @brief Associates an existing customer with an open receipt for cashback purposes.
 *
 * Sets the receipt's customer ID to the specified customer and persists the change.
 *
 * @param receipt_id ID of the receipt to update.
 * @param customer_id ID of the customer to associate with the receipt.
 * @throws std::runtime_error If the receipt is closed.
 * @throws std::runtime_error If the customer does not exist.
 */
void ReceiptService::AddCustomerToReceipt(int receipt_id, int customer_id)
{
    Receipt receipt = m_ReceiptStorage->GetReceipt(receipt_id);
    if(receipt.GetStatus() == ReceiptStatus::CLOSED) throw std::runtime_error("Can't change customer for closed receipt");
    std::optional<Customer> customer = m_CashbackService->GetCustomer(customer_id);
    if(!customer.has_value()) throw std::runtime_error("Customer does not exist");
    receipt.SetCustomerID(customer->GetID());
    m_ReceiptStorage->UpdateReceipt(receipt);
}

/**
 * @brief Closes a receipt, optionally applies cashback, awards cashback to the customer, and persists the updated receipt.
 *
 * If `use_cashback` is non-zero, a customer must be associated with the receipt and the specified amount
 * will be deducted from that customer's cashback balance. If a customer is present, the receipt's total
 * amount will be credited as new cashback. The receipt's status is set to `CLOSED` and the change is saved.
 *
 * @param receipt_id ID of the receipt to close.
 * @param use_cashback Amount of cashback to apply to the receipt; must be greater than or equal to zero.
 * @throws std::runtime_error if `use_cashback` is less than zero or if `use_cashback` is non-zero but the receipt has no associated customer.
 * @throws std::exception Propagates exceptions thrown by the cashback service when applying cashback; if applying cashback fails the receipt's used cashback is reset before the exception propagates.
 */
void ReceiptService::CloseReceipt(int receipt_id, double use_cashback)
{
    if(use_cashback < 0) throw std::runtime_error("Cashback shouldn't be less than 0");

    Receipt receipt = m_ReceiptStorage->GetReceipt(receipt_id);
    std::optional<int> customer_id = receipt.GetCustomerID();

    if(!customer_id.has_value() && use_cashback != 0) throw std::runtime_error("Customer ID should be not null to use cashback");
    
    if(customer_id.has_value() && use_cashback != 0)
    {
        receipt.SetUsedCashback(use_cashback);
        try
        {
            m_CashbackService->UseCashback(customer_id.value(), use_cashback);
        }
        catch(const std::exception& e)
        {
            receipt.SetUsedCashback(0);
            throw;
        }
    }

    if(customer_id.has_value())
    {
        m_CashbackService->AddCashback(customer_id.value(), receipt.CalcTotal());
    }

    receipt.SetStatus(ReceiptStatus::CLOSED);
    m_ReceiptStorage->UpdateReceipt(receipt);
}

/**
 * @brief Cancels an opened receipt, restores stock for each item, and removes the receipt.
 *
 * @param receipt_id ID of the receipt to cancel.
 * @throws std::runtime_error if the receipt is not in OPENED status.
 * @throws std::exception Propagates exceptions thrown by the product/receipt storage or services.
 */
void ReceiptService::CancelReceipt(int receipt_id)
{
    Receipt receipt = m_ReceiptStorage->GetReceipt(receipt_id);
    if(receipt.GetStatus() != ReceiptStatus::OPENED) throw std::runtime_error("Receipt should be opened to cancel it");
    for(auto item : receipt.GetItems())
    {
        m_ProductService->AddStock(item.GetProduct().GetID(), item.GetQuantity());
    }
    m_ReceiptStorage->RemoveReceipt(receipt_id);
}

std::optional<Receipt> ReceiptService::GetReceipt(int receipt_id)
{
    try
    {
        return m_ReceiptStorage->GetReceipt(receipt_id);
    }
    catch(const std::exception& e)
    {
        return std::nullopt;
    }
    
}

std::vector<Receipt> ReceiptService::GetAllReceipts()
{
    return m_ReceiptStorage->GetAllReceipts();
}