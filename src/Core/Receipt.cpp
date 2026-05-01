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

time_t Receipt::GetTimestamp()
{
    return m_Timestamp;
}

std::vector<ReceiptItem> Receipt::GetItems()
{
    return m_Items;
}

void Receipt::SetID(int id)
{
    m_ID = id;
}

void Receipt::AddItem(ReceiptItem item)
{
    m_Items.push_back(item);
}

double Receipt::CalcTotal()
{
    double total = 0;
    for(auto& item : m_Items)
    {
        total += item.CalcPrice();
    }
    return total;
}
