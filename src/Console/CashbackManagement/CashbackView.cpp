#include "Console/TerminalStyle.h"
#include "Console/CashbackManagement/CashbackView.h"
#include "Console/SafeRead.h"
#include "Console/MainView.h"
#include "Console/CashbackManagement/ListCustomersView.h"
#include "Console/CashbackManagement/EditCustomerView.h"
#include "Bussiness/RegularCashback.h"

CashbackView::CashbackView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> CashbackView::Run()
{
    std::cout << "===== Cashback & Customers =====\n";
    std::cout << "Choose the option:\n";
    std::cout << "  1. List customers\n";
    std::cout << "  2. Add customer\n";
    std::cout << "  3. Edit customer\n";
    std::cout << "  4. Back\n";
    std::cout << "\n";

    int choice = 0;
    while(!(SafeRead("Option", choice) && choice >= 1 && choice <= 4))
    {
        TerminalStyle::PrintError("Invalid option");
    }

    switch (choice)
    {
    case 1:
        std::cout << "\n";
        return std::make_unique<ListCustomersView>(m_Context);
        break;

    case 2:
    {
        std::string name, surname;
        SafeRead("Name", name);
        SafeRead("Surname", surname);

        try
        {
            m_Context.cashback_service->CreateCustomer(name, surname, std::make_shared<RegularCashback>());
        }
        catch(const std::exception& e)
        {
            TerminalStyle::PrintError(e.what());
        }

        std::cout << "\n";
        return std::make_unique<CashbackView>(m_Context);
        break;
    }

    case 3: {
        int customer_id;
        while(!SafeRead("Customer ID", customer_id))
        {
            TerminalStyle::PrintError("ID should be an integer");
        }
        
        std::cout << "\n";
        return std::make_unique<EditCustomerView>(m_Context, customer_id);
        break;
    }
    
    default:
        std::cout << "\n";
        return std::make_unique<MainView>(m_Context);
        break;
    }
}