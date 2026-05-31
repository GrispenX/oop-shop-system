#include "Console/TerminalStyle.h"
#include "Console/CashbackManagement/EditCustomerView.h"
#include "Console/CashbackManagement/CashbackView.h"
#include "Console/SafeRead.h"
#include <iostream>
#include <iomanip>

EditCustomerView::EditCustomerView(Context context, int customer_id) :
    m_Context(context),
    m_CustomerID(customer_id)
{
    
}

std::unique_ptr<IView> EditCustomerView::Run()
{
    std::optional<Customer> customer = m_Context.cashback_service->GetCustomer(m_CustomerID);
    if(!customer.has_value())
    {
        TerminalStyle::PrintError("Customer not found");
        return std::make_unique<CashbackView>(m_Context);
    }

    std::cout << "===== Customer =====\n";
    std::cout << "ID:               " << customer->GetID() << "\n";
    std::cout << "Name:             " << customer->GetName() << "\n";
    std::cout << "Surname:          " << customer->GetSurname() << "\n";
    std::cout << "Cashback balance: " << customer->GetBalance() << "\n";
    std::cout << "\n";
    std::cout << "Choose the option:\n";
    std::cout << "  1. Set name\n";
    std::cout << "  2. Set surname\n";
    std::cout << "  3. Back\n";
    std::cout << "\n";

    int choice = 0;
    while(!(SafeRead("Option", choice) && choice >= 1 && choice <= 3))
    {
        TerminalStyle::PrintError("Invalid option");
    }

    switch (choice)
    {
    case 1: {
        std::string new_name;
        SafeRead("New name", new_name);
        
        try
        {
            m_Context.cashback_service->SetCustomerName(m_CustomerID, new_name);
        }
        catch(const std::exception& e)
        {
            TerminalStyle::PrintError(e.what());
        }

        std::cout << "\n";
        return std::make_unique<EditCustomerView>(m_Context, m_CustomerID);
        break;
    }

    case 2: {
        std::string new_surname;
        SafeRead("New surname", new_surname);

        try
        {
            m_Context.cashback_service->SetCustomerSurname(m_CustomerID, new_surname);
        }
        catch(const std::exception& e)
        {
            TerminalStyle::PrintError(e.what());
        }

        std::cout << "\n";
        return std::make_unique<EditCustomerView>(m_Context, m_CustomerID);
        break;
    }
    
    default:
        std::cout << "\n";
        return std::make_unique<CashbackView>(m_Context);
        break;
    }
}