#include "Console/ReceiptManagement/ReceiptDetailsView.h"
#include "Console/ReceiptManagement/SelectReceiptView.h"
#include "Console/ReceiptManagement/ReceiptsView.h"
#include "Console/SafeRead.h"
#include "Console/TerminalStyle.h"
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
        TerminalStyle::PrintError("Receipt not found");
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

    int choice = 0;
    while(!(SafeRead("Option", choice) && choice >= 1 && choice <= 5))
    {
        TerminalStyle::PrintError("Invalid option");
    }
    
    switch (choice)
    {
    case 1: {
        int product_id, quantity;
        while (!SafeRead("Product ID", product_id))
        {
            TerminalStyle::PrintError("ID should be an integer");
        }
        while (!SafeRead("Quantity", quantity))
        {
            TerminalStyle::PrintError("Quantity should be an integer");
        }

        try
        {
            m_Context.receipt_service->AddItemToReceipt(m_ReceiptID, product_id, quantity);
        }
        catch(const std::exception& e)
        {
            TerminalStyle::PrintError(e.what());
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
                while (!SafeRead("How much cashback whould you like to use", use_cashback))
                {
                    TerminalStyle::PrintError("Cashback should be a number");
                }
            }
        }

        try
        {
            m_Context.receipt_service->CloseReceipt(m_ReceiptID, use_cashback);
        }
        catch(const std::exception& e)
        {
            TerminalStyle::PrintError(e.what());
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
            TerminalStyle::PrintError(e.what());
        }

        std::cout << "\n";
        return std::make_unique<ReceiptsView>(m_Context);
        break;
    
    case 4: {
        int customer_id;
        while (!SafeRead("Customer ID", customer_id))
        {
            TerminalStyle::PrintError("ID should be an integer");
        }
        
        try
        {
            m_Context.receipt_service->AddCustomerToReceipt(m_ReceiptID, customer_id);
        }
        catch(const std::exception& e)
        {
            TerminalStyle::PrintError(e.what());
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