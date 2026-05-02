#ifndef SRC_CORE_RECEIPT_H_
#define SRC_CORE_RECEIPT_H_

#include <ctime>
#include <vector>
#include "Core/ReceiptItem.h"

enum class ReceiptStatus
{
    OPENED,
    CLOSED
};

class Receipt
{
public:
    Receipt(int id, time_t timestamp, std::vector<ReceiptItem> items, ReceiptStatus status);
    
    int GetID() const;
    time_t GetTimestamp() const;
    ReceiptStatus GetStatus() const;
    std::vector<ReceiptItem> GetItems() const;

    void SetID(int id);
    void SetTimestamp(time_t timestamp);
    void SetStatus(ReceiptStatus status);
    
    void AddItem(ReceiptItem item);
    double CalcTotal() const;

private:
    int m_ID;
    time_t m_Timestamp;
    ReceiptStatus m_Status;
    std::vector<ReceiptItem> m_Items;
};

#endif // SRC_CORE_RECEIPT_H_