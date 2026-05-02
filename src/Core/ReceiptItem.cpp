#include "Core/ReceiptItem.h"

ReceiptItem::ReceiptItem(Product product, int quantity) :
    m_Product(product),
    m_Quantity(quantity)
{
    if(quantity <= 0) throw std::runtime_error("Quantity should be positive");
}

Product ReceiptItem::GetProduct() const
{
    return m_Product;
}

int ReceiptItem::GetQuantity() const
{
    return m_Quantity;
}

double ReceiptItem::CalcPrice() const
{
    std::shared_ptr<IDiscountStrategy> discount_strat = m_Product.GetDiscount();
    double discount = discount_strat ? discount_strat->CalcDiscount(m_Product.GetPrice(), m_Quantity) : 0.0;
    return m_Product.GetPrice() * m_Quantity - discount;
}