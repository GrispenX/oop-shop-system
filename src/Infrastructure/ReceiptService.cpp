#include "Infrastructure/ReceiptService.h"

ReceiptService::ReceiptService(std::shared_ptr<IProductStorage> product_storage, std::shared_ptr<IReceiptStorage> receipt_storage) :
    m_ProductStorage(product_storage),
    m_ReceiptStorage(receipt_storage)
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
    ReceiptItem item(product, quantity);
    receipt.AddItem(item);
    m_ReceiptStorage->UpdateReceipt(receipt);
}

void ReceiptService::CloseReceipt(int receipt_id)
{
    Receipt receipt = m_ReceiptStorage->GetReceipt(receipt_id);
    receipt.SetStatus(ReceiptStatus::CLOSED);
    m_ReceiptStorage->UpdateReceipt(receipt);
}

void ReceiptService::CancelReceipt(int receipt_id)
{
    Receipt receipt = m_ReceiptStorage->GetReceipt(receipt_id);
    if(receipt.GetStatus() != ReceiptStatus::OPENED) throw std::runtime_error("Receipt should be opened to cancel it");
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