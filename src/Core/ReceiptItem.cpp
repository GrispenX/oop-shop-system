#include "Core/ReceiptItem.h"

ReceiptItem::ReceiptItem(Product product, int quantity, std::unique_ptr<IDiscountStrategy> discount) :
    m_Product(product),
    m_Quantity(quantity),
    m_Discount(std::move(discount))
{
    if(quantity <= 0) throw std::runtime_error("Quantity should be positive");
}

double ReceiptItem::GetPrice()
{
    double discount = m_Discount ? m_Discount->CalcDiscount(m_Product.GetPrice(), m_Quantity) : 0.0;
    return m_Product.GetPrice() * m_Quantity - discount;
}