#include "Core/Receipt.h"

std::ostream& operator<<(std::ostream& os, ReceiptStatus status)
{
    switch (status)
    {
    case ReceiptStatus::OPENED:
        os << "Opened";
        break;

    case ReceiptStatus::CLOSED:
        os << "Closed";
        break;
    
    default:
        os << "Oops";
        break;
    }
    return os;
}

Receipt::Receipt(
    int id,
    time_t timestamp,
    std::vector<ReceiptItem> items,
    ReceiptStatus status,
    std::optional<int> customer_id,
    double used_cashback
) :
    m_ID(id),
    m_Timestamp(timestamp),
    m_Items(items),
    m_Status(status),
    m_CustomerID(customer_id),
    m_UsedCashback(used_cashback)
{
    if(used_cashback != 0 && !customer_id.has_value()) throw std::runtime_error("Customer should be not null to use cashback");
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

std::optional<int> Receipt::GetCustomerID() const
{
    return m_CustomerID;
}

double Receipt::GetUsedCashback() const
{
    return m_UsedCashback;
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

void Receipt::SetCustomerID(std::optional<int> customer_id)
{
    m_CustomerID = customer_id;
}

void Receipt::SetUsedCashback(double used_cashback)
{
    if(used_cashback < 0) throw std::runtime_error("Cashback should be greater than 0");
    if(used_cashback > CalcTotal()) throw std::runtime_error("Cashback shouldn't be greater than total price");
    m_UsedCashback = used_cashback;
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
