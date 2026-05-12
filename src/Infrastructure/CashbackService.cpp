#include "Infrastructure/CashbackService.h"

/**
 * @brief Constructs a CashbackService using the provided customer storage.
 *
 * @param customer_storage Shared pointer to an ICustomerStorage implementation used for persisting and retrieving Customer entities.
 */
CashbackService::CashbackService(std::shared_ptr<ICustomerStorage> customer_storage) :
    m_CustomerStorage(customer_storage)
{

}

/**
 * @brief Creates a new customer with the provided personal details and cashback strategy and persists it.
 *
 * @param name Customer's first name.
 * @param surname Customer's last name.
 * @param cashback_strategy Strategy used to calculate this customer's cashback.
 * @return int Identifier of the newly created customer.
 */
int CashbackService::CreateCustomer(std::string name, std::string surname, std::shared_ptr<ICashbackStrategy> cashback_strategy)
{
    Customer customer(0, name, surname, 0, cashback_strategy);
    return m_CustomerStorage->Add(customer);
}

/**
 * @brief Retrieve a customer by identifier.
 *
 * @param customer_id Identifier of the customer to retrieve.
 * @return std::optional<Customer> A `Customer` wrapped in `std::optional` if retrieval succeeds and the customer exists; `std::nullopt` if the customer is not found or an exception occurs during retrieval.
 */
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

/**
 * @brief Retrieve all customers from persistent storage.
 *
 * @return std::vector<Customer> A vector containing every stored Customer.
 */
std::vector<Customer> CashbackService::GetAllCustomers()
{
    return m_CustomerStorage->GetAll();
}

/**
 * @brief Update the given customer's first name and persist the change.
 *
 * @param customer_id Identifier of the customer to update.
 * @param name New given name for the customer.
 */
void CashbackService::SetCustomerName(int customer_id, std::string name)
{
    Customer customer = m_CustomerStorage->Get(customer_id);
    customer.SetName(name);
    m_CustomerStorage->Update(customer);
}

/**
 * @brief Updates the surname of the specified customer and persists the change.
 *
 * @param customer_id Identifier of the customer whose surname will be updated.
 * @param surname New surname to set for the customer.
 */
void CashbackService::SetCustomerSurname(int customer_id, std::string surname)
{
    Customer customer = m_CustomerStorage->Get(customer_id);
    customer.SetSurname(surname);
    m_CustomerStorage->Update(customer);
}

/**
 * @brief Attempts to deduct a specified cashback amount from a customer's balance.
 *
 * @param customer_id Identifier of the customer whose balance will be modified.
 * @param amount Amount of cashback to deduct; must be greater than or equal to 0.
 *
 * @throws std::runtime_error If `amount` is less than 0.
 * @throws std::runtime_error If the customer's balance is less than `amount`.
 */
void CashbackService::UseCashback(int customer_id, double amount)
{
    if(amount < 0) throw std::runtime_error("Amount shouldn't be less than 0");
    Customer customer = m_CustomerStorage->Get(customer_id);
    double balance = customer.GetBalance();
    if(balance < amount) throw std::runtime_error("There is not enough cashback on balance");
    customer.SetBalance(balance - amount);
    m_CustomerStorage->Update(customer);
}

/**
 * @brief Adds cashback to a customer's balance based on a receipt total and persists the change.
 *
 * Computes cashback using the customer's cashback strategy, increases the customer's balance by that amount,
 * and updates the customer in storage.
 *
 * @param customer_id Identifier of the customer to update.
 * @param receipt_total Total amount of the receipt used to calculate cashback; must be greater than or equal to 0.
 * @throws std::runtime_error If `receipt_total` is less than 0.
 */
void CashbackService::AddCashback(int customer_id, double receipt_total)
{
    if(receipt_total < 0) throw std::runtime_error("Receipt total shouldn't be less than 0");
    Customer customer = m_CustomerStorage->Get(customer_id);
    double balance = customer.GetBalance();
    double to_add = customer.GetCashbackStrategy()->CalcCashback(receipt_total);
    customer.SetBalance(balance + to_add);
    m_CustomerStorage->Update(customer);
}