#include "Core/Receipt.h"
#include "Bussiness/RegularDiscount.h"
#include "Bussiness/BundleDiscount.h"
#include <iostream>

int main()
{
    Product wine(1, "Wine", 200, std::make_shared<RegularDiscount>(0.2));
    Product bread(2, "Bread", 30, std::make_shared<BundleDiscount>(3, 0.5));

    ReceiptItem item1(wine, 5);
    ReceiptItem item2(bread, 8);

    Receipt receipt(1, time(nullptr), {item1, item2});

    std::cout << receipt.GetTotal() << "\n";
}