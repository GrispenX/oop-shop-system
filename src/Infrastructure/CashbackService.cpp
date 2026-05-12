#include "Infrastructure/CashbackService.h"

CashbackService::CashbackService(std::shared_ptr<ICustomerStorage> customer_storage) :
    m_CustomerStorage(customer_storage)
{

}

int CashbackService::CreateCustomer(std::string name, std::string surname, std::shared_ptr<ICashbackStrategy> cashback_strategy)
{
    Customer customer(0, name, surname, 0, cashback_strategy);
    return m_CustomerStorage->Add(customer);
}

std::optional<Customer> CashbackService::GetCustomer(int customer_id)
{
    try
    {
        return m_CustomerStorage->Get(customer_id);
    }
    catch(const std::exception& e)
    {
        return std::nullopt;
    }
    
}

std::vector<Customer> CashbackService::GetAllCustomers()
{
    return m_CustomerStorage->GetAll();
}

void CashbackService::UseCashback(int customer_id, double amount)
{
    Customer customer = m_CustomerStorage->Get(customer_id);
    double balance = customer.GetBalance();
    if(balance < amount) throw std::runtime_error("There is not enough cashback on balance");
    customer.SetBalance(balance - amount);
    m_CustomerStorage->Update(customer);
}

void CashbackService::AddCashback(int customer_id, double receipt_total)
{
    Customer customer = m_CustomerStorage->Get(customer_id);
    double balance = customer.GetBalance();
    double to_add = customer.GetCashbackStrategy()->CalcCashback(receipt_total);
    customer.SetBalance(balance + to_add);
    m_CustomerStorage->Update(customer);
}