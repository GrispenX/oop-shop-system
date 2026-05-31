#include "Console/ProductManagement/SelectProductView.h"
#include "Console/ProductManagement/EditProductView.h"
#include "Console/SafeRead.h"
#include "Console/TerminalStyle.h"
#include <iostream>

SelectProductView::SelectProductView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> SelectProductView::Run()
{
    int id;
    while(!SafeRead("Product ID", id))
    {
        TerminalStyle::PrintError("ID should be an integer");
    }

    std::optional<Product> product = m_Context.product_service->GetProduct(id);

    if(product.has_value())
    {
        std::cout << "\n";
        return std::make_unique<EditProductView>(m_Context, id);
    }
    else
    {
        TerminalStyle::PrintError("Product not found");
        return std::make_unique<SelectProductView>(m_Context);
    }
}