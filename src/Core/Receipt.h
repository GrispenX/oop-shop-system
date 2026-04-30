#ifndef SRC_CORE_RECEIPT_H_
#define SRC_CORE_RECEIPT_H_

#include <ctime>
#include <vector>
#include "Core/ReceiptItem.h"

class Receipt
{
public:
    Receipt(int id, time_t timestamp, std::vector<ReceiptItem> items);
    int GetID();
    void AddItem(ReceiptItem item);
    double GetTotal();

private:
    int m_ID;
    time_t m_Timestamp;
    std::vector<ReceiptItem> m_Items;
};

#endif // SRC_CORE_RECEIPT_H_