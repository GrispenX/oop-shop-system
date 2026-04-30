#include "Core/Receipt.h"
#include "Bussiness/RegularDiscount.h"
#include "Bussiness/BundleDiscount.h"
#include <iostream>

int main()
{
    IDiscountStrategy* regular = new RegularDiscount(0.2);
    IDiscountStrategy* bundle = new BundleDiscount(3, 0.5);

    Product wine("Wine", 200);
    Product bread("Bread", 30);

    ReceiptItem item1(wine, 5, std::make_unique<RegularDiscount>(0.2));
    ReceiptItem item2(bread, 8, std::make_unique<BundleDiscount>(3, 0.5));

    Receipt receipt;
    receipt.AddItem(std::move(item1));
    receipt.AddItem(std::move(item2));

    std::cout << receipt.GetTotal() << "\n";
}