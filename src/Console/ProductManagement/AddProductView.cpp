#include "Console/ProductManagement/AddProductView.h"
#include "Console/ProductManagement/ProductsView.h"
#include <iostream>

AddProductView::AddProductView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> AddProductView::Run()
{
    std::string name;
    double price;

    std::cout << "Name: ";
    std::cin >> name;
    std::cout << "Price: ";
    std::cin >> price;

    try
    {
        m_Context.product_service->CreateProduct(name, price);
    }
    catch(const std::exception& e)
    {
        std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
    }
    
    std::cout << "\n";
    return std::make_unique<ProductsView>(m_Context);
}