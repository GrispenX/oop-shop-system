#include "Console/TerminalStyle.h"
#include "Console/SafeRead.h"
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
    SafeRead("Name", name);
    while(!SafeRead("Price", price))
    {
        TerminalStyle::PrintError("Price should be a number");
    }

    try
    {
        m_Context.product_service->CreateProduct(name, price);
    }
    catch(const std::exception& e)
    {
        TerminalStyle::PrintError(e.what());
    }
    
    std::cout << "\n";
    return std::make_unique<ProductsView>(m_Context);
}