#include "Bussiness/DiscountService.h"

DiscountService::DiscountService(std::shared_ptr<IProductStorage> product_storage) :
    m_ProductStorage(product_storage)
{

}

void DiscountService::SetDiscount(int product_id, std::shared_ptr<IDiscountStrategy> discount)
{
    Product product = m_ProductStorage->GetProduct(product_id);
    product.SetDiscount(discount);
    m_ProductStorage->UpdateProduct(product);
}

std::vector<Product> DiscountService::GetDiscountedProducts()
{
    return m_ProductStorage->GetProducts([](Product product) {
        return product.GetDiscount() != nullptr;
    });
}
