#include "Console/CashbackManagement/EditCustomerView.h"
#include "Console/CashbackManagement/CashbackView.h"
#include "Console/ReadWithValidation.h"
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
        std::cout << "\033[1;31m" << "Customer not found" << "\033[0m\n\n";
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

    int choice = ReadWithValidation<int>("Choice", [](int i) { return i >= 1 && i <= 3; });

    switch (choice)
    {
    case 1: {
        std::string new_name;
        std::cout << "New name: ";
        std::cin >> new_name;
        m_Context.cashback_service->SetCustomerName(m_CustomerID, new_name);
        std::cout << "\n";
        return std::make_unique<EditCustomerView>(m_Context, m_CustomerID);
        break;
    }

    case 2: {
        std::string new_surname;
        std::cout << "New surname: ";
        std::cin >> new_surname;
        m_Context.cashback_service->SetCustomerSurname(m_CustomerID, new_surname);
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