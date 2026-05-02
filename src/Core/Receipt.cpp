#include "Core/Receipt.h"

Receipt::Receipt(int id, time_t timestamp, std::vector<ReceiptItem> items, ReceiptStatus status) :
    m_ID(id),
    m_Timestamp(timestamp),
    m_Items(items),
    m_Status(status)
{

}

int Receipt::GetID() const
{
    return m_ID;
}

time_t Receipt::GetTimestamp() const
{
    return m_Timestamp;
}

ReceiptStatus Receipt::GetStatus() const
{
    return m_Status;
}

std::vector<ReceiptItem> Receipt::GetItems() const
{
    return m_Items;
}

void Receipt::SetID(int id)
{
    m_ID = id;
}

void Receipt::SetTimestamp(time_t timestamp)
{
    m_Timestamp = timestamp;
}

void Receipt::SetStatus(ReceiptStatus status)
{
    m_Status = status;
}

void Receipt::AddItem(ReceiptItem item)
{
    m_Items.push_back(item);
}

double Receipt::CalcTotal() const
{
    double total = 0;
    for(const auto& item : m_Items)
    {
        total += item.CalcPrice();
    }
    return total;
}
