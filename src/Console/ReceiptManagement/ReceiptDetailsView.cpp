#include "Console/ReceiptManagement/ReceiptDetailsView.h"
#include "Console/ReceiptManagement/SelectReceiptView.h"
#include "Console/ReceiptManagement/ReceiptsView.h"
#include "Console/ReadWithValidation.h"
#include <iostream>
#include <iomanip>
#include <format>

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
        if(item.GetProduct().GetDiscount() != nullptr)
        {
            std::cout << "\t\t\t" << std::setw(20) << item.GetProduct().GetDiscount()->GetDescription() << "\n";
        }
    }

    std::optional<int> customer_id = r.GetCustomerID();
    if(customer_id.has_value())
    {
        std::optional<Customer> customer = m_Context.cashback_service->GetCustomer(customer_id.value());
        if(customer.has_value())
        {
            std::cout << "Customer: " << std::format("{}. {} {}\n", customer->GetID(), customer->GetName(), customer->GetSurname());
            if(r.GetUsedCashback() > 0)
            {
                std::cout << "Used cashback: " << r.GetUsedCashback() << "$\n";
            }
        }
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
    std::cout << "  4. Set customer\n";
    std::cout << "  5. Back\n";
    std::cout << "\n";

    int choice = ReadWithValidation<int>("Choice", [](int i) { return i >= 1 && i <= 5; });
    
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

    case 2: {
        double use_cashback = 0;
        if(customer_id.has_value())
        {
            std::optional<Customer> customer = m_Context.cashback_service->GetCustomer(customer_id.value());
            if(customer.has_value())
            {
                std::cout << "How much cashback would you like to use: ";
                std::cin >> use_cashback;
            }
        }
        try
        {
            m_Context.receipt_service->CloseReceipt(m_ReceiptID, use_cashback);
        }
        catch(const std::exception& e)
        {
            std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
            std::cout << "\n";
            return std::make_unique<ReceiptDetailsView>(m_Context, m_ReceiptID);
        }
        std::cout << "\n";
        return std::make_unique<ReceiptsView>(m_Context);
        break;
    }

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
    
    case 4: {
        int customer_id;
        std::cout << "Customer ID: ";
        std::cin >> customer_id;
        try
        {
            m_Context.receipt_service->AddCustomerToReceipt(m_ReceiptID, customer_id);
        }
        catch(const std::exception& e)
        {
            std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
        }
        std::cout << "\n";
        return std::make_unique<ReceiptDetailsView>(m_Context, m_ReceiptID);
        break;
    }

    default:
        std::cout << "\n";
        return std::make_unique<ReceiptsView>(m_Context);
        break;
    }
}