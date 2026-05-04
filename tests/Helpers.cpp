#include "../tests/Helpers.h"

bool operator==(const Product& p1, const Product& p2)
{
    return p1.GetID() == p2.GetID() &&
           p1.GetName() == p2.GetName() &&
           p1.GetPrice() == p2.GetPrice() &&
           p1.GetDiscount() == p2.GetDiscount();
}

bool operator==(const ReceiptItem& i1, const ReceiptItem& i2)
{
    return i1.GetProduct() == i2.GetProduct() &&
           i1.GetQuantity() == i2.GetQuantity();
}

bool operator==(const Receipt& r1, const Receipt& r2)
{
    auto items1 = r1.GetItems();
    auto items2 = r2.GetItems();
    if(items1.size() == items2.size())
    {
        for(int i = 0; i < items1.size(); i++)
        {
            if(items1[i] != items2[i])
            {
                return false;
            }
        }
    }

    return r1.GetID() == r2.GetID() &&
           r1.GetStatus() == r2.GetStatus() &&
           r1.GetTimestamp() == r2.GetTimestamp();
}