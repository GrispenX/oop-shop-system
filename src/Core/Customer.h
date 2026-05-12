#ifndef SRC_CORE_CUSTOMER_H_
#define SRC_CORE_CUSTOMER_H_

#include "Core/ICashbackStrategy.h"
#include <string>
#include <memory>

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