#ifndef SRC_CORE_RECEIPT_H_
#define SRC_CORE_RECEIPT_H_

#include <ctime>
#include <vector>
#include <optional>
#include "Core/ReceiptItem.h"
#include "Core/Customer.h"

enum class ReceiptStatus
{
    OPENED,
    CLOSED
};

std::ostream& operator<<(std::ostream& os, ReceiptStatus status);

class Receipt
{
public:
    Receipt(
        int id,
        time_t timestamp,
        std::vector<ReceiptItem> items,
        ReceiptStatus status,
        std::optional<int> customer_id = std::nullopt,
        double used_cashback = 0
    );
    
    int GetID() const;
    time_t GetTimestamp() const;
    ReceiptStatus GetStatus() const;
    std::vector<ReceiptItem> GetItems() const;
    std::optional<int> GetCustomerID() const;
    double GetUsedCashback() const;

    void SetID(int id);
    void SetTimestamp(time_t timestamp);
    void SetStatus(ReceiptStatus status);
    void SetCustomerID(std::optional<int> customer_id);
    void SetUsedCashback(double used_cashback);
    
    void AddItem(ReceiptItem item);
    // Excluding cashback
    double CalcTotal() const;

private:
    int m_ID;
    time_t m_Timestamp;
    ReceiptStatus m_Status;
    std::vector<ReceiptItem> m_Items;
    std::optional<int> m_CustomerID;
    double m_UsedCashback;
};

#endif // SRC_CORE_RECEIPT_H_