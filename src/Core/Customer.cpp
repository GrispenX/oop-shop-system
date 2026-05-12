#include "Core/Customer.h"
#include <stdexcept>

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

int Customer::GetID() const
{
    return m_ID;
}

std::string Customer::GetName() const
{
    return m_Name;

}

std::string Customer::GetSurname() const
{
    return m_Surname;
}

double Customer::GetBalance() const
{
    return m_CashbackBalance;
}

std::shared_ptr<ICashbackStrategy> Customer::GetCashbackStrategy() const
{
    return m_CashbackStrategy;
}

void Customer::SetID(int id)
{
    m_ID = id;
}

void Customer::SetName(std::string name)
{
    m_Name = name;
}

void Customer::SetSurname(std::string surname)
{
    m_Surname = surname;
}

void Customer::SetBalance(double balance)
{
    if(balance < 0) throw std::runtime_error("Cashback balance can't be less than 0");
    m_CashbackBalance = balance;
}

void Customer::SetCashbackStrategy(std::shared_ptr<ICashbackStrategy> strategy)
{
    if(strategy == nullptr) throw std::runtime_error("Customer should have cashback strategy");
    m_CashbackStrategy = strategy;
}