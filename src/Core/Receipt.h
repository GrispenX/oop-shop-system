#ifndef SRC_CORE_RECEIPT_H_
#define SRC_CORE_RECEIPT_H_

#include <ctime>
#include <vector>
#include "Core/ReceiptItem.h"

class Receipt
{
public:
    void AddItem(ReceiptItem item);
    double GetTotal();

private:
    time_t m_Timestamp;
    std::vector<ReceiptItem> m_Items;
};

#endif // SRC_CORE_RECEIPT_H_