#include "Console/ProductManagement/EditProductView.h"
#include "Console/ProductManagement/SelectProductView.h"
#include "Console/ProductManagement/ProductsView.h"
#include "Console/SafeRead.h"
#include "Console/TerminalStyle.h"
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
        TerminalStyle::PrintError("Product not found");
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

    int choice = 0;
    while(!(SafeRead("Option", choice) && choice >= 1 && choice <= 5))
    {
        TerminalStyle::PrintError("Invalid option");
    }
    
    switch(choice)
    {
    case 1: {
        std::string name;
        SafeRead("New name", name);

        try
        {
            m_Context.product_service->SetName(m_ProductID, name);
        }
        catch(const std::exception& e)
        {
            TerminalStyle::PrintError(e.what());
        }
        
        std::cout << "\n";
        return std::make_unique<EditProductView>(m_Context, m_ProductID);
        break;
    }

    case 2:
        double price;
        while(!SafeRead("Price", price))
        {
            TerminalStyle::PrintError("Price should be a number");
        }

        try
        {
            m_Context.product_service->SetPrice(m_ProductID, price);
        }
        catch(const std::exception& e)
        {
            TerminalStyle::PrintError(e.what());
        }

        std::cout << "\n";
        return std::make_unique<EditProductView>(m_Context, m_ProductID);
        break;

    case 3:
        int amount;
        while(!SafeRead("Amount", price))
        {
            TerminalStyle::PrintError("Amount should be an integer");
        }

        try
        {
            m_Context.product_service->AddStock(m_ProductID, amount);
        }
        catch(const std::exception& e)
        {
            TerminalStyle::PrintError(e.what());
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
            while(!(SafeRead("Discount type (1 - Regular, 2 - Bundle)", discount_type) && discount_type >= 1 && discount_type <= 2))
            {
                TerminalStyle::PrintError("Invalid option");
            }

            if(discount_type == 1)
            {
                double percentage;
                while(!SafeRead("Percentage (%)", percentage))
                {
                    TerminalStyle::PrintError("Percentage should be a number");
                }

                try
                {
                    m_Context.product_service->SetDiscount(m_ProductID, std::make_shared<RegularDiscount>(percentage / 100.0));
                }
                catch(const std::exception& e)
                {
                    TerminalStyle::PrintError(e.what());
                }
            }
            else if(discount_type == 2)
            {
                double percentage;
                int quantity;
                while(!SafeRead("Percentage (%)", percentage))
                {
                    TerminalStyle::PrintError("Percentage should be a number");
                }
                while (!SafeRead("Quantity", quantity))
                {
                    TerminalStyle::PrintError("Quantity should be an integer");
                }
                
                try
                {
                    m_Context.product_service->SetDiscount(m_ProductID, std::make_shared<BundleDiscount>(quantity, percentage / 100.0));
                }
                catch(const std::exception& e)
                {
                    TerminalStyle::PrintError(e.what());
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