#include "Bussiness/ReceiptService.h"

ReceiptService::ReceiptService(std::shared_ptr<IProductStorage> product_storage, std::shared_ptr<IReceiptStorage> receipt_storage) :
    m_ProductStorage(product_storage),
    m_ReceiptStorage(receipt_storage)
{

}

int ReceiptService::CreateReceipt(std::vector<std::pair<int, int>> items)
{
    std::vector<ReceiptItem> items_to_add;
    for(auto& [id, quantity] : items)
    {
        ReceiptItem item(m_ProductStorage->GetProduct(id), quantity);
        items_to_add.push_back(item);
    }
    Receipt receipt(0, time(nullptr), items_to_add);
    return m_ReceiptStorage->AddReceipt(receipt);
}

std::vector<Receipt> ReceiptService::GetAllReceipts()
{
    return m_ReceiptStorage->GetAllReceipts();
}