#include "Console/ReceiptManagement/ReceiptDetailsView.h"
#include "Console/ReceiptManagement/SelectReceiptView.h"
#include "Console/ReceiptManagement/ReceiptsView.h"
#include "Console/ReadWithValidation.h"
#include <iostream>
#include <iomanip>

ReceiptDetailsView::ReceiptDetailsView(Context context, int receipt_id) :
    m_Context(context),
    m_ReceiptID(receipt_id)
{

}

std::unique_ptr<IView> ReceiptDetailsView::Run()
{
    std::optional<Receipt> receipt = m_Context.receipt_service->GetReceipt(m_ReceiptID);
    if(!receipt.has_value())
    {
        std::cout << "\033[1;31mReceipt not found!\033[0m\n";
        return std::make_unique<SelectReceiptView>(m_Context);
    }
    Receipt r = receipt.value();

    std::time_t timestamp = r.GetTimestamp();
    std::tm* local_timestamp = std::localtime(&timestamp);
    char str_time[20];
    std::strftime(str_time, sizeof(str_time), "%Y-%m-%d %T", local_timestamp);

    std::cout << "===== Receipt details =====\n";
    std::cout << "ID:        " << r.GetID() << "\n";
    std::cout << "Status:    " << r.GetStatus() << "\n";
    std::cout << "Timestamp: " << str_time << "\n";

    for(auto& item : r.GetItems())
    {
        std::cout << "\t";
        std::cout << std::setw(3) << item.GetQuantity() << "x ";
        std::cout << std::setw(20) << item.GetProduct().GetName() << " ";
        std::cout << std::setw(10) << item.CalcPrice() << "$\n";
    }

    std::cout << "Total:     " << r.CalcTotal() << "\n";

    if(r.GetStatus() != ReceiptStatus::OPENED)
    {
        std::cout << "\n";
        return std::make_unique<ReceiptsView>(m_Context);
    }

    std::cout << "\n";
    std::cout << "  1. Add item\n";
    std::cout << "  2. Close receipt\n";
    std::cout << "  3. Cancel receipt\n";
    std::cout << "  4. Back\n";
    std::cout << "\n";

    int choice = ReadWithValidation<int>("Choice", [](int i) { return i >= 1 && i <= 4; });
    
    switch (choice)
    {
    case 1: {
        int product_id;
        int quantity;
        std::cout << "Product ID: ";
        std::cin >> product_id;
        std::cout << "Quantity: ";
        std::cin >> quantity;
        try
        {
            m_Context.receipt_service->AddItemToReceipt(m_ReceiptID, product_id, quantity);
        }
        catch(const std::exception& e)
        {
            std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
        }
        std::cout << "\n";
        return std::make_unique<ReceiptDetailsView>(m_Context, m_ReceiptID);
        break;
    }

    case 2:
        try
        {
            m_Context.receipt_service->CloseReceipt(m_ReceiptID);
        }
        catch(const std::exception& e)
        {
            std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
        }
        std::cout << "\n";
        return std::make_unique<ReceiptsView>(m_Context);
        break;

    case 3:
        try
        {
            m_Context.receipt_service->CancelReceipt(m_ReceiptID);
        }
        catch(const std::exception& e)
        {
            std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
        }
        std::cout << "\n";
        return std::make_unique<ReceiptsView>(m_Context);
        break;
    
    default:
        std::cout << "\n";
        return std::make_unique<ReceiptsView>(m_Context);
        break;
    }
}