#ifndef SRC_CORE_CUSTOMER_H_
#define SRC_CORE_CUSTOMER_H_

#include "Core/ICashbackStrategy.h"
#include <string>
#include <memory>

/**
 * Represents a customer with identity, personal details, a cashback balance, and an associated cashback strategy.
 */
 
/**
 * Initializes a Customer with identity, name, surname, initial cashback balance, and a cashback strategy.
 * @param id Numeric identifier for the customer.
 * @param name Customer's given name.
 * @param surname Customer's family name.
 * @param balance Initial cashback balance for the customer.
 * @param cashback_strategy Shared pointer to the customer's cashback strategy implementation.
 */

/**
 * Returns the customer's numeric identifier.
 * @returns The customer's ID.
 */

/**
 * Returns the customer's given name.
 * @returns The customer's name.
 */

/**
 * Returns the customer's family name.
 * @returns The customer's surname.
 */

/**
 * Returns the customer's cashback balance.
 * @returns The customer's cashback balance.
 */

/**
 * Returns the customer's configured cashback strategy.
 * @returns Shared pointer to the customer's cashback strategy.
 */

/**
 * Sets the customer's numeric identifier.
 * @param id New ID to assign to the customer.
 */

/**
 * Sets the customer's given name.
 * @param name New name to assign to the customer.
 */

/**
 * Sets the customer's family name.
 * @param surname New surname to assign to the customer.
 */

/**
 * Sets the customer's cashback balance.
 * @param balance New cashback balance to assign to the customer.
 */

/**
 * Sets the customer's cashback strategy.
 * @param strategy Shared pointer to the new cashback strategy to assign.
 */
class Customer
{
public:
    Customer(int id, std::string name, std::string surname, double balance, std::shared_ptr<ICashbackStrategy> cashback_strategy);

    int GetID() const;
    std::string GetName() const;
    std::string GetSurname() const;
    double GetBalance() const;
    std::shared_ptr<ICashbackStrategy> GetCashbackStrategy() const;
    
    void SetID(int id);
    void SetName(std::string name);
    void SetSurname(std::string surname);
    void SetBalance(double balance);
    void SetCashbackStrategy(std::shared_ptr<ICashbackStrategy> strategy);
    
private:
    int m_ID;
    std::string m_Name;
    std::string m_Surname;
    double m_CashbackBalance;
    std::shared_ptr<ICashbackStrategy> m_CashbackStrategy;
};

#endif // SRC_CORE_CUSTOMER_H_