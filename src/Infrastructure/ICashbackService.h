#ifndef SRC_INFRASTRUCTURE_ICASHBACKSERVICE_H_
#define SRC_INFRASTRUCTURE_ICASHBACKSERVICE_H_

#include "Core/Customer.h"
#include <vector>
#include <optional>

class ICashbackService
{
public:
    virtual ~ICashbackService() = default;

    virtual int CreateCustomer(std::string name, std::string surname, std::shared_ptr<ICashbackStrategy> cashback_strategy) = 0;
    virtual std::optional<Customer> GetCustomer(int customer_id) = 0;
    virtual std::vector<Customer> GetAllCustomers() = 0;
    // virtual void SetCustomerName(std::string name) = 0;
    // virtual void SetCustomerSurname(std::string surname) = 0;
    // virtual void SetCustomerCashbackStrategy(std::shared_ptr<ICashbackStrategy> strategy) = 0;
    virtual void UseCashback(int customer_id, double amount) = 0;
    virtual void AddCashback(int customer_id, double receipt_total) = 0;
};

#endif // SRC_INFRASTRUCTURE_ICASHBACKSERVICE_H_