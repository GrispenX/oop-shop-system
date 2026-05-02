#include "Console/ProductManagement/ProductsView.h"
#include "Console/ProductManagement/ListProductsView.h"
#include "Console/ProductManagement/AddProductView.h"
#include "Console/ProductManagement/SelectProductView.h"
#include "Console/MainView.h"
#include "Console/ReadWithValidation.h"
#include <iostream>

ProductsView::ProductsView(Context context) :
    m_Context(context)
{
    
}

std::unique_ptr<IView> ProductsView::Run()
{
    std::cout << "===== Products =====\n";
    std::cout << "Choose the option:\n";
    std::cout << "  1. List products\n";
    std::cout << "  2. Add product\n";
    std::cout << "  3. Edit product\n";
    std::cout << "  4. Back\n";
    std::cout << "\n";

    int choice = ReadWithValidation<int>("Choice", [](int input) { return input >= 1 && input <= 4; });

    switch (choice)
    {
    case 1:
        std::cout << "\n";
        return std::make_unique<ListProductsView>(m_Context);
        break;

    case 2:
        return std::make_unique<AddProductView>(m_Context);
        break;

    case 3:
        return std::make_unique<SelectProductView>(m_Context);
        break;
    
    default:
        std::cout << "\n";
        return std::make_unique<MainView>(m_Context);
        break;
    }
}