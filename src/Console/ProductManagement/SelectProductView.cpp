#include "Console/ProductManagement/SelectProductView.h"
#include "Console/ProductManagement/EditProductView.h"
#include <iostream>

SelectProductView::SelectProductView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> SelectProductView::Run()
{
    int id;
    std::cout << "Product ID: ";
    std::cin >> id;

    std::optional<Product> product = m_Context.product_service->GetProduct(id);

    if(product.has_value())
    {
        std::cout << "\n";
        return std::make_unique<EditProductView>(m_Context, id);
    }
    else
    {
        std::cout << "\033[1;31mProduct not found! Try again\033[0m\n";
        return std::make_unique<SelectProductView>(m_Context);
    }
}