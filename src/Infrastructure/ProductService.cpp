#include "Infrastructure/ProductService.h"

ProductService::ProductService(std::shared_ptr<IProductStorage> product_storage) :
    m_ProductStorage(product_storage)
{

}

int ProductService::CreateProduct(std::string name, double price)
{
    Product product(0, name, price, nullptr);
    return m_ProductStorage->AddProduct(product);
}

void ProductService::SetPrice(int product_id, double price)
{
    Product product = m_ProductStorage->GetProduct(product_id);
    product.SetPrice(price);
    m_ProductStorage->UpdateProduct(product);
}

void ProductService::SetName(int product_id, std::string name)
{
    Product product = m_ProductStorage->GetProduct(product_id);
    product.SetName(name);
    m_ProductStorage->UpdateProduct(product);
}

std::optional<Product> ProductService::GetProduct(int product_id)
{
    try
    {
        return m_ProductStorage->GetProduct(product_id);
    }
    catch(const std::exception& e)
    {
        return std::nullopt;
    }
}

std::vector<Product> ProductService::GetAll()
{
    return m_ProductStorage->GetAllProducts();
}

void ProductService::SetDiscount(int product_id, std::shared_ptr<IDiscountStrategy> discount)
{
    Product product = m_ProductStorage->GetProduct(product_id);
    product.SetDiscount(discount);
    m_ProductStorage->UpdateProduct(product);
}