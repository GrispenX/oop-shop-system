#include "Console/CashbackManagement/CashbackView.h"
#include "Console/ReadWithValidation.h"
#include "Console/MainView.h"
#include "Console/CashbackManagement/ListCustomersView.h"
#include "Console/CashbackManagement/EditCustomerView.h"
#include "Bussiness/RegularCashback.h"

/**
 * @brief Constructs a CashbackView and stores the provided application context.
 *
 * @param context Application context containing services and configuration used by the view; copied into the view's internal `m_Context`.
 */
CashbackView::CashbackView(Context context) :
    m_Context(context)
{

}

/**
 * @brief Displays the "Cashback & Customers" menu, reads a validated choice, and transitions to the selected view.
 *
 * Presents options to list customers, add a customer, edit a customer, or go back. When adding a customer the method prompts for name and surname before attempting creation.
 *
 * @return std::unique_ptr<IView> Pointer to the next view:
 * - ListCustomersView for option 1.
 * - CashbackView (re-displays this menu) after attempting to add a customer (option 2).
 * - EditCustomerView constructed with the entered customer ID for option 3.
 * - MainView for option 4 (back).
 */
std::unique_ptr<IView> CashbackView::Run()
{
    std::cout << "===== Cashback & Customers =====\n";
    std::cout << "Choose the option:\n";
    std::cout << "  1. List customers\n";
    std::cout << "  2. Add customer\n";
    std::cout << "  3. Edit customer\n";
    std::cout << "  4. Back\n";
    std::cout << "\n";

    int choice = ReadWithValidation<int>("Choice", [](int i) { return i >= 1 && i <= 4; });

    switch (choice)
    {
    case 1:
        std::cout << "\n";
        return std::make_unique<ListCustomersView>(m_Context);
        break;

    case 2:
    {
        std::string name, surname;
        std::cout << "Name: ";
        std::cin >> name;
        std::cout << "Surname: ";
        std::cin >> surname;
        try
        {
            m_Context.cashback_service->CreateCustomer(name, surname, std::make_shared<RegularCashback>());
        }
        catch(const std::exception& e)
        {
            std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
        }
        std::cout << "\n";
        return std::make_unique<CashbackView>(m_Context);
        break;
    }

    case 3: {
        int customer_id;
        std::cout << "Customer ID: ";
        std::cin >> customer_id;
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