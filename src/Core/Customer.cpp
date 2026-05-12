#include "Core/Customer.h"
#include <stdexcept>

/**
 * @brief Initialize a Customer with identification, name, surname, cashback balance, and cashback strategy.
 *
 * @param id Customer identifier.
 * @param name Customer given name.
 * @param surname Customer family name.
 * @param balance Initial cashback balance; must be greater than or equal to 0.
 * @param cashback_strategy Strategy used to calculate cashback; must not be null.
 *
 * @throws std::runtime_error if `balance` is less than 0.
 * @throws std::runtime_error if `cashback_strategy` is null.
 */
Customer::Customer(int id, std::string name, std::string surname, double balance, std::shared_ptr<ICashbackStrategy> cashback_strategy) :
    m_ID(id),
    m_Name(name),
    m_Surname(surname),
    m_CashbackBalance(balance),
    m_CashbackStrategy(cashback_strategy)
{
    if(balance < 0) throw std::runtime_error("Cashback balance can't be less than 0");
    if(cashback_strategy == nullptr) throw std::runtime_error("Customer should have cashback strategy");
}

/**
 * @brief Retrieves the customer's identifier.
 *
 * @return int The customer's ID.
 */
int Customer::GetID() const
{
    return m_ID;
}

/**
 * @brief Retrieves the customer's name.
 *
 * @return The customer's name.
 */
std::string Customer::GetName() const
{
    return m_Name;

}

/**
 * @brief Retrieves the customer's surname.
 *
 * @return std::string The customer's surname.
 */
std::string Customer::GetSurname() const
{
    return m_Surname;
}

/**
 * @brief Retrieves the customer's cashback balance.
 *
 * @return Current cashback balance.
 */
double Customer::GetBalance() const
{
    return m_CashbackBalance;
}

/**
 * @brief Retrieves the customer's cashback strategy.
 *
 * @return std::shared_ptr<ICashbackStrategy> Shared pointer to the customer's cashback strategy.
 */
std::shared_ptr<ICashbackStrategy> Customer::GetCashbackStrategy() const
{
    return m_CashbackStrategy;
}

/**
 * @brief Sets the customer's identifier.
 *
 * @param id New customer identifier.
 */
void Customer::SetID(int id)
{
    m_ID = id;
}

/**
 * @brief Sets the customer's given name.
 *
 * @param name New given name for the customer.
 */
void Customer::SetName(std::string name)
{
    m_Name = name;
}

/**
 * @brief Sets the customer's surname.
 *
 * @param surname New surname for the customer.
 */
void Customer::SetSurname(std::string surname)
{
    m_Surname = surname;
}

/**
 * @brief Sets the customer's cashback balance.
 *
 * @param balance New cashback balance; must be greater than or equal to 0.
 * @throws std::runtime_error if `balance` is less than 0.
 */
void Customer::SetBalance(double balance)
{
    if(balance < 0) throw std::runtime_error("Cashback balance can't be less than 0");
    m_CashbackBalance = balance;
}

/**
 * Assigns a cashback strategy to the customer.
 *
 * @param strategy Shared pointer to an ICashbackStrategy to use for the customer; must not be `nullptr`.
 * @throws std::runtime_error if `strategy` is `nullptr` (message: "Customer should have cashback strategy").
 */
void Customer::SetCashbackStrategy(std::shared_ptr<ICashbackStrategy> strategy)
{
    if(strategy == nullptr) throw std::runtime_error("Customer should have cashback strategy");
    m_CashbackStrategy = strategy;
}