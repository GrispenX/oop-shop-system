#include "Infrastructure/ReceiptService.h"

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

void ReceiptService::AddCustomerToReceipt(int receipt_id, int customer_id)
{
    Receipt receipt = m_ReceiptStorage->GetReceipt(receipt_id);
    if(receipt.GetStatus() == ReceiptStatus::CLOSED) throw std::runtime_error("Can't change customer for closed receipt");
    std::optional<Customer> customer = m_CashbackService->GetCustomer(customer_id);
    if(!customer.has_value()) throw std::runtime_error("Customer does not exist");
    receipt.SetCustomerID(customer->GetID());
    m_ReceiptStorage->UpdateReceipt(receipt);
}

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