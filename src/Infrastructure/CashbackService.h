#ifndef SRC_INFRASTRUCTURE_CASHBACKSERVICE_H_
#define SRC_INFRASTRUCTURE_CASHBACKSERVICE_H_

#include "Infrastructure/ICashbackService.h"
#include "Core/ICustomerStorage.h"

class CashbackService : public ICashbackService
{
public:
    CashbackService(std::shared_ptr<ICustomerStorage> customer_storage);

    int CreateCustomer(std::string name, std::string surname, std::shared_ptr<ICashbackStrategy> cashback_strategy) override;
    std::optional<Customer> GetCustomer(int customer_id) override;
    std::vector<Customer> GetAllCustomers() override;
    void UseCashback(int customer_id, double amount) override;
    void AddCashback(int customer_id, double receipt_total) override;

private:
    std::shared_ptr<ICustomerStorage> m_CustomerStorage;
};

#endif // SRC_INFRASTRUCTURE_CASHBACKSERVICE_H_