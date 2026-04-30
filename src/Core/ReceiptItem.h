#ifndef SRC_CORE_RECEIPTITEM_H_
#define SRC_CORE_RECEIPTITEM_H_

#include "Core/Product.h"
#include "Core/IDiscountStrategy.h"
#include <memory>

class ReceiptItem
{
public:
    ReceiptItem(Product product, int quantity, std::unique_ptr<IDiscountStrategy> discount);
    double GetPrice();

private:
    Product m_Product;
    int m_Quantity;
    std::unique_ptr<IDiscountStrategy>m_Discount;
};

#endif // SRC_CORE_RECEIPTITEM_H_