#include "Core/Receipt.h"

/**
 * @brief Appends a textual representation of a ReceiptStatus to an output stream.
 *
 * @param os Output stream to which the status text will be appended.
 * @param status ReceiptStatus value to format.
 * @return std::ostream& The same output stream `os` after the status text has been written.
 */
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

/**
 * @brief Constructs a receipt with the specified identifier, timestamp, items, status, optional customer, and applied cashback.
 *
 * @param id Receipt identifier.
 * @param timestamp Creation timestamp.
 * @param items List of receipt items.
 * @param status Receipt status.
 * @param customer_id Optional customer identifier associated with the receipt.
 * @param used_cashback Amount of cashback applied to the receipt; zero if none.
 *
 * @throws std::runtime_error if `used_cashback` is non‑zero while `customer_id` is not set.
 */
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

/**
 * @brief Gets the receipt identifier.
 *
 * @return int The receipt's identifier.
 */
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

/**
 * @brief Retrieve a copy of the receipt's line items.
 *
 * @return std::vector<ReceiptItem> A vector containing copies of the receipt's items.
 */
std::vector<ReceiptItem> Receipt::GetItems() const
{
    return m_Items;
}

/**
 * @brief Retrieves the associated customer ID for the receipt.
 *
 * @return std::optional<int> containing the customer ID when set, empty optional otherwise.
 */
std::optional<int> Receipt::GetCustomerID() const
{
    return m_CustomerID;
}

/**
 * @brief Gets the amount of cashback applied to the receipt.
 *
 * @return double Amount of cashback applied to this receipt.
 */
double Receipt::GetUsedCashback() const
{
    return m_UsedCashback;
}

/**
 * @brief Sets the receipt's identifier.
 *
 * @param id New receipt identifier.
 */
void Receipt::SetID(int id)
{
    m_ID = id;
}

void Receipt::SetTimestamp(time_t timestamp)
{
    m_Timestamp = timestamp;
}

/**
 * @brief Sets the receipt's status.
 *
 * @param status New status to assign to the receipt.
 */
void Receipt::SetStatus(ReceiptStatus status)
{
    m_Status = status;
}

/**
 * @brief Associates the receipt with a customer or clears the association.
 *
 * @param customer_id Optional customer ID to set; use `std::nullopt` to remove any existing customer association.
 */
void Receipt::SetCustomerID(std::optional<int> customer_id)
{
    m_CustomerID = customer_id;
}

/**
 * @brief Set the amount of cashback applied to the receipt.
 *
 * @param used_cashback Amount of cashback to apply; must be greater than or equal to 0 and less than or equal to the receipt total.
 *
 * @throws std::runtime_error if `used_cashback` is negative.
 * @throws std::runtime_error if `used_cashback` is greater than the receipt total.
 */
void Receipt::SetUsedCashback(double used_cashback)
{
    if(used_cashback < 0) throw std::runtime_error("Cashback should be greater than 0");
    if(used_cashback > CalcTotal()) throw std::runtime_error("Cashback shouldn't be greater than total price");
    m_UsedCashback = used_cashback;
}

/**
 * @brief Appends an item to the receipt's item list.
 *
 * @param item ReceiptItem to add; it is copied into the receipt's internal item collection.
 */
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
