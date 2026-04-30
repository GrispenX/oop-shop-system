#include "Core/Receipt.h"

Receipt::Receipt(int id, time_t timestamp, std::vector<ReceiptItem> items) :
    m_ID(id),
    m_Timestamp(timestamp),
    m_Items(items)
{

}

int Receipt::GetID()
{
    return m_ID;
}

void Receipt::AddItem(ReceiptItem item)
{
    m_Items.push_back(item);
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