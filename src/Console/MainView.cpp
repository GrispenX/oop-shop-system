#include "Console/MainView.h"
#include "Console/ProductManagement/ProductsView.h"
#include "Console/ReceiptManagement/ReceiptsView.h"
#include "Console/CashbackManagement/CashbackView.h"
#include "Console/ReadWithValidation.h"
#include <iostream>

MainView::MainView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> MainView::Run()
{
    std::cout << "===== ATP Management System =====\n";
    std::cout << "Choose the option:\n";
    std::cout << "  1. Products\n";
    std::cout << "  2. Receipts\n";
    std::cout << "  3. Cashback & customers\n";
    std::cout << "  4. Quit\n";
    std::cout << "\n";

    int choice = ReadWithValidation<int>("Choice", [](int i) { return i >= 1 && i <= 4; });

    std::cout << "\n";
    
    switch (choice)
    {
    case 1:
        return std::make_unique<ProductsView>(m_Context);
        break;

    case 2:
        return std::make_unique<ReceiptsView>(m_Context);
        break;

    case 3:
        return std::make_unique<CashbackView>(m_Context);
        break;
    
    default:
        return nullptr;
        break;
    }
}