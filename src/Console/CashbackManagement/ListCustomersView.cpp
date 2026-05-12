#include "Console/CashbackManagement/ListCustomersView.h"
#include "Console/CashbackManagement/CashbackView.h"
#include <iostream>
#include <iomanip>
#include <format>

/**
 * @brief Constructs a view that lists customers using the provided application context.
 *
 * Stores the given Context in the view for use when rendering the customers list.
 *
 * @param context Application context providing services and configuration required by the view.
 */
ListCustomersView::ListCustomersView(Context context) :
    m_Context(context)
{

}

/**
 * @brief Displays the customers list on the console and advances to the cashback view.
 *
 * Prints a table header and a row for each customer obtained from the current context's
 * cashback service, then returns control to the cashback view.
 *
 * @return std::unique_ptr<IView> Owner of a CashbackView initialized with the same Context.
 */
std::unique_ptr<IView> ListCustomersView::Run()
{
    std::cout << "===== Customers list =====\n";

    std::cout << std::setw(4) << "ID" << " ";
    std::cout << std::setw(20) << "Name" << " ";
    std::cout << std::setw(20) << "Surname" << " ";
    std::cout << std::setw(10) << "Balance" << "\n";
    for(int i = 0; i < 4 + 20 + 20 + 10 + 3; i++) std::cout << '-';
    std::cout << "\n";

    for(auto& c : m_Context.cashback_service->GetAllCustomers())
    {
        std::cout << std::setw(4) << c.GetID() << " ";
        std::cout << std::setw(20) << c.GetName() << " ";
        std::cout << std::setw(20) << c.GetSurname() << " ";
        std::cout << std::setw(10) << std::format("{}$", c.GetBalance()) << "\n";
    }
    std::cout << "\n";

    return std::make_unique<CashbackView>(m_Context);
}