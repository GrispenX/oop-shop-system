#include "Console/ProductManagement/EditProductView.h"
#include "Console/ProductManagement/SelectProductView.h"
#include "Console/ProductManagement/ProductsView.h"
#include "Console/ReadWithValidation.h"
#include <iostream>
#include <format>

EditProductView::EditProductView(Context context, int product_id) :
    m_Context(context),
    m_ProductID(product_id)
{

}

std::unique_ptr<IView> EditProductView::Run()
{
    std::optional<Product> product = m_Context.product_service->GetProduct(m_ProductID);
    if(!product.has_value())
    {
        std::cout << "\033[1;31mProduct not found!\033[0m\n";
        return std::make_unique<SelectProductView>(m_Context);
    }
    Product p = product.value();

    std::cout << "===== Product management =====\n";
    std::cout << "ID:    " << p.GetID() << "\n";
    std::cout << "Name:  " << p.GetName() << "\n";
    std::cout << "Price: " << p.GetPrice() << "\n";
    std::cout << "\n";
    std::cout << "  1. Change name\n";
    std::cout << "  2. Change price\n";
    std::cout << "  3. Back\n";
    std::cout << "\n";

    int choice = ReadWithValidation<int>("Choice", [](int i) { return i >= 1 && i <= 3; });
    
    switch(choice)
    {
    case 1: {
        std::string name;
        std::cout << "New name: ";
        std::cin >> name;
        m_Context.product_service->SetName(m_ProductID, name);
        std::cout << "\n";
        return std::make_unique<EditProductView>(m_Context, m_ProductID);
        break;
    }

    case 2:
        double price;
        std::cout << "NewPrice: ";
        std::cin >> price;
        try
        {
            m_Context.product_service->SetPrice(m_ProductID, price);
        }
        catch(const std::exception& e)
        {
            std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
        }
        std::cout << "\n";
        return std::make_unique<EditProductView>(m_Context, m_ProductID);
        break;
    
    default:
        std::cout << "\n";
        return std::make_unique<ProductsView>(m_Context);
        break;
    }
}