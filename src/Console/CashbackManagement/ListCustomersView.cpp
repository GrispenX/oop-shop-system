#include "Console/CashbackManagement/ListCustomersView.h"
#include "Console/CashbackManagement/CashbackView.h"
#include <iostream>
#include <iomanip>
#include <format>

ListCustomersView::ListCustomersView(Context context) :
    m_Context(context)
{

}

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