#include "Console/ProductManagement/ListProductsView.h"
#include "Console/ProductManagement/ProductsView.h"
#include <iostream>
#include <format>
#include <iomanip>

ListProductsView::ListProductsView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> ListProductsView::Run()
{
    std::cout << "===== Products list =====\n";

    std::cout << std::setw(4) << "ID" << " ";
    std::cout << std::setw(20) << "Name" << " ";
    std::cout << std::setw(10) << "Price" << "\n";
    for(int i = 0; i < 4 + 20 + 10 + 2; i++) std::cout << "-";
    std::cout << "\n";

    for(auto& p : m_Context.product_service->GetAll())
    {
        std::cout << std::setw(4) << p.GetID() << "|";
        std::cout << std::setw(20) << p.GetName() << "|";
        std::cout << std::setw(10) << std::format("{}$", p.GetPrice()) << "\n";
    }

    std::cout << "\n";

    return std::make_unique<ProductsView>(m_Context);
}