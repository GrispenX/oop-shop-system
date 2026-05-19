#include "Console/ProductManagement/EditProductView.h"
#include "Console/ProductManagement/SelectProductView.h"
#include "Console/ProductManagement/ProductsView.h"
#include "Console/ReadWithValidation.h"
#include "Bussiness/RegularDiscount.h"
#include "Bussiness/BundleDiscount.h"
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
    std::cout << "ID:       " << p.GetID() << "\n";
    std::cout << "Name:     " << p.GetName() << "\n";
    std::cout << "Price:    " << p.GetPrice() << "\n";
    std::cout << "In stock: " << m_Context.product_service->GetStockAmount(p.GetID()) << "\n";
    if(p.GetDiscount() != nullptr)
    {
        std::cout << "Discount: " << p.GetDiscount()->GetDescription() << "\n";
    }
    std::cout << "\n";
    std::cout << "  1. Change name\n";
    std::cout << "  2. Change price\n";
    std::cout << "  3. Restock\n";
    if(p.GetDiscount() != nullptr)
    {
        std::cout << "  4. Remove discount\n";
    }
    else
    {
        std::cout << "  4. Set discount\n";
    }
    std::cout << "  5. Back\n";
    std::cout << "\n";

    int choice = ReadWithValidation<int>("Choice", [](int i) { return i >= 1 && i <= 5; });
    
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

    case 3:
        double amount;
        std::cout << "Amount: ";
        std::cin >> amount;
        try
        {
            m_Context.product_service->AddStock(m_ProductID, amount);
        }
        catch(const std::exception& e)
        {
            std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
        }
        std::cout << "\n";
        return std::make_unique<EditProductView>(m_Context, m_ProductID);
        break;

    case 4:
        if(p.GetDiscount() != nullptr)
        {
            m_Context.product_service->SetDiscount(m_ProductID, nullptr);
            return std::make_unique<EditProductView>(m_Context, m_ProductID);
        }
        else
        {
            int discount_type = ReadWithValidation<int>("Discount type (1 - Regular, 2 - Bundle)", [](int i) {return i >= 1 && i <= 2;});
            if(discount_type == 1)
            {
                double percentage;
                std::cout << "Percentage (%): ";
                std::cin >> percentage;
                try
                {
                    m_Context.product_service->SetDiscount(m_ProductID, std::make_shared<RegularDiscount>(percentage / 100.0));
                }
                catch(const std::exception& e)
                {
                    std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
                }
                
            }
            else if(discount_type == 2)
            {
                double percentage;
                int quantity;
                std::cout << "Percentage (%): ";
                std::cin >> percentage;
                std::cout << "Quantity: ";
                std::cin >> quantity;
                try
                {
                    m_Context.product_service->SetDiscount(m_ProductID, std::make_shared<BundleDiscount>(quantity, percentage / 100.0));
                }
                catch(const std::exception& e)
                {
                    std::cout << "\033[1;31m" << e.what() << "\033[0m\n";
                }
            }
            std::cout << "\n";
            return std::make_unique<EditProductView>(m_Context, m_ProductID);
        }
        break;
    
    default:
        std::cout << "\n";
        return std::make_unique<ProductsView>(m_Context);
        break;
    }
}