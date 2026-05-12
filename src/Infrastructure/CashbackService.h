#ifndef SRC_INFRASTRUCTURE_CASHBACKSERVICE_H_
#define SRC_INFRASTRUCTURE_CASHBACKSERVICE_H_

#include "Infrastructure/ICashbackService.h"
#include "Core/ICustomerStorage.h"

/**
 * Service that manages customers and their cashback operations.
 */

/**
 * Construct a CashbackService with the given customer storage backend.
 * @param customer_storage Storage used to persist and retrieve customers.
 */

/**
 * Create a new customer with the provided name, surname, and cashback strategy.
 * @param name Customer's given name.
 * @param surname Customer's family name.
 * @param cashback_strategy Strategy used to calculate/assign cashback for the customer.
 * @returns New customer's integer identifier.
 */

/**
 * Retrieve a customer by identifier.
 * @param customer_id Identifier of the customer to retrieve.
 * @returns `std::optional<Customer>` containing the customer if found, empty otherwise.
 */

/**
 * Retrieve all customers.
 * @returns Vector of all stored customers.
 */

/**
 * Update the stored given name for a customer.
 * @param customer_id Identifier of the customer to update.
 * @param name New given name for the customer.
 */

/**
 * Update the stored family name for a customer.
 * @param customer_id Identifier of the customer to update.
 * @param surname New family name for the customer.
 */

/**
 * Deduct a specified amount from the customer's available cashback.
 * @param customer_id Identifier of the customer whose cashback will be used.
 * @param amount Amount of cashback to consume.
 */

/**
 * Add cashback to a customer's balance based on a receipt total.
 * @param customer_id Identifier of the customer to credit.
 * @param receipt_total Total value of the receipt used to compute cashback.
 */
class CashbackService : public ICashbackService
{
public:
    CashbackService(std::shared_ptr<ICustomerStorage> customer_storage);

    int CreateCustomer(std::string name, std::string surname, std::shared_ptr<ICashbackStrategy> cashback_strategy) override;
    std::optional<Customer> GetCustomer(int customer_id) override;
    std::vector<Customer> GetAllCustomers() override;
    void SetCustomerName(int customer_id, std::string name) override;
    void SetCustomerSurname(int customer_id, std::string surname) override;
    void UseCashback(int customer_id, double amount) override;
    void AddCashback(int customer_id, double receipt_total) override;

private:
    std::shared_ptr<ICustomerStorage> m_CustomerStorage;
};

#endif // SRC_INFRASTRUCTURE_CASHBACKSERVICE_H_