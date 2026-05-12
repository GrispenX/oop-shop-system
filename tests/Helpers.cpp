#include "../tests/Helpers.h"

bool operator==(const Product& p1, const Product& p2)
{
    return p1.GetID() == p2.GetID() &&
           p1.GetName() == p2.GetName() &&
           p1.GetPrice() == p2.GetPrice() &&
           p1.GetDiscount() == p2.GetDiscount();
}

bool operator==(const ReceiptItem& i1, const ReceiptItem& i2)
{
    return i1.GetProduct() == i2.GetProduct() &&
           i1.GetQuantity() == i2.GetQuantity();
}

/**
 * @brief Checks equality of two Receipt objects.
 *
 * Compares the receipts' item lists when both have the same number of items (using `ReceiptItem` equality),
 * and always compares receipt metadata: ID, status, and timestamp. If item counts differ, item lists are not compared;
 * equality is determined solely by the metadata comparison.
 *
 * @param r1 First receipt to compare.
 * @param r2 Second receipt to compare.
 * @return `true` if the receipts are considered equal according to the above rules, `false` otherwise.
 */
bool operator==(const Receipt& r1, const Receipt& r2)
{
    auto items1 = r1.GetItems();
    auto items2 = r2.GetItems();
    if(items1.size() == items2.size())
    {
        for(int i = 0; i < items1.size(); i++)
        {
            if(items1[i] != items2[i])
            {
                return false;
            }
        }
    }

    return r1.GetID() == r2.GetID() &&
           r1.GetStatus() == r2.GetStatus() &&
           r1.GetTimestamp() == r2.GetTimestamp();
}

/**
 * @brief Determines whether two Customer objects represent the same customer by comparing their ID, name, surname, balance, and cashback strategy.
 *
 * @param c1 First customer to compare.
 * @param c2 Second customer to compare.
 * @return true if all compared fields (ID, name, surname, balance, cashback strategy) are equal, false otherwise.
 */
bool operator==(const Customer& c1, const Customer& c2)
{
    return c1.GetID() == c2.GetID() &&
           c1.GetName() == c2.GetName() &&
           c1.GetSurname() == c2.GetSurname() &&
           c1.GetBalance() == c2.GetBalance() &&
           c1.GetCashbackStrategy() == c2.GetCashbackStrategy();
}