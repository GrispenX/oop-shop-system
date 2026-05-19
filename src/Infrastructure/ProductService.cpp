#include "Infrastructure/ProductService.h"

ProductService::ProductService(std::shared_ptr<IProductStorage> product_storage, std::shared_ptr<IInventoryStorage> inventory_storage) :
    m_ProductStorage(product_storage),
    m_InventoryStorage(inventory_storage)
{

}

int ProductService::CreateProduct(std::string name, double price)
{
    Product product(0, name, price, nullptr);
    int id = m_ProductStorage->AddProduct(product);
    m_InventoryStorage->SetStock(id, 0);
    return id;
}

void ProductService::SetPrice(int product_id, double price)
{
    std::optional<Product> product = m_ProductStorage->GetProduct(product_id);
    if(!product) throw std::runtime_error("Product not found");
    product->SetPrice(price);
    m_ProductStorage->UpdateProduct(product.value());
}

void ProductService::SetName(int product_id, std::string name)
{
    std::optional<Product> product = m_ProductStorage->GetProduct(product_id);
    if(!product) throw std::runtime_error("Product not found");
    product->SetName(name);
    m_ProductStorage->UpdateProduct(product.value());
}

std::optional<Product> ProductService::GetProduct(int product_id)
{
    return m_ProductStorage->GetProduct(product_id);
}

std::vector<Product> ProductService::GetAll()
{
    return m_ProductStorage->GetAllProducts();
}

void ProductService::SetDiscount(int product_id, std::shared_ptr<IDiscountStrategy> discount)
{
    std::optional<Product> product = m_ProductStorage->GetProduct(product_id);
    if(!product) throw std::runtime_error("Product not found");
    product->SetDiscount(discount);
    m_ProductStorage->UpdateProduct(product.value());
}

int ProductService::GetStockAmount(int product_id)
{
    if(!m_ProductStorage->GetProduct(product_id)) throw std::runtime_error("Product not found");
    try
    {
        return m_InventoryStorage->GetStock(product_id);
    }
    catch(const std::exception& e)
    {
        m_InventoryStorage->SetStock(product_id, 0);
        return 0;
    }
}

void ProductService::AddStock(int product_id, int amount)
{
    if(!m_ProductStorage->GetProduct(product_id)) throw std::runtime_error("Product not found");
    if(amount <= 0) throw std::runtime_error("Amount should be greater than 0");
    try
    {
        int in_stock = m_InventoryStorage->GetStock(product_id);
        m_InventoryStorage->SetStock(product_id, in_stock + amount);
    }
    catch(const std::exception& e)
    {
        m_InventoryStorage->SetStock(product_id, amount);
    }
}

void ProductService::RemoveStock(int product_id, int amount)
{
    if(!m_ProductStorage->GetProduct(product_id)) throw std::runtime_error("Product not found");
    if(amount <= 0) throw std::runtime_error("Amount should be greater than 0");
    int in_stock;
    try
    {
        in_stock = m_InventoryStorage->GetStock(product_id);
    }
    catch(const std::exception& e)
    {
        m_InventoryStorage->SetStock(product_id, 0);
        in_stock = 0;
    }
    if(in_stock - amount < 0) throw std::runtime_error("There is not enough product in stock");
    m_InventoryStorage->SetStock(product_id, in_stock - amount);
}