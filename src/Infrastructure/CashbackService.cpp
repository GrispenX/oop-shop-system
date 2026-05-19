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
    return m_CustomerStorage->Get(customer_id);
}

std::vector<Customer> CashbackService::GetAllCustomers()
{
    return m_CustomerStorage->GetAll();
}

void CashbackService::SetCustomerName(int customer_id, std::string name)
{
    std::optional<Customer> customer = m_CustomerStorage->Get(customer_id);
    if(!customer) throw std::runtime_error("Customer not found");
    customer->SetName(name);
    m_CustomerStorage->Update(customer.value());
}

void CashbackService::SetCustomerSurname(int customer_id, std::string surname)
{
    std::optional<Customer> customer = m_CustomerStorage->Get(customer_id);
    if(!customer) throw std::runtime_error("Customer not found");
    customer->SetSurname(surname);
    m_CustomerStorage->Update(customer.value());
}

void CashbackService::UseCashback(int customer_id, double amount)
{
    if(amount < 0) throw std::runtime_error("Amount shouldn't be less than 0");
    std::optional<Customer> customer = m_CustomerStorage->Get(customer_id);
    if(!customer) throw std::runtime_error("Customer not found");

    double balance = customer->GetBalance();
    if(balance < amount) throw std::runtime_error("There is not enough cashback on balance");
    customer->SetBalance(balance - amount);
    m_CustomerStorage->Update(customer.value());
}

void CashbackService::AddCashback(int customer_id, double receipt_total)
{
    if(receipt_total < 0) throw std::runtime_error("Receipt total shouldn't be less than 0");
    std::optional<Customer> customer = m_CustomerStorage->Get(customer_id);
    if(!customer) throw std::runtime_error("Customer not found");

    double balance = customer->GetBalance();
    double to_add = customer->GetCashbackStrategy()->CalcCashback(receipt_total);
    customer->SetBalance(balance + to_add);
    m_CustomerStorage->Update(customer.value());
}