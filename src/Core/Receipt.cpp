#include "Core/Receipt.h"

void Receipt::AddItem(ReceiptItem item)
{
    m_Items.push_back(std::move(item));
}

double Receipt::GetTotal()
{
    double total = 0;
    for(auto& item : m_Items)
    {
        total += item.GetPrice();
    }
    return total;
}