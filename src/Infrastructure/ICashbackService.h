#ifndef SRC_INFRASTRUCTURE_ICASHBACKSERVICE_H_
#define SRC_INFRASTRUCTURE_ICASHBACKSERVICE_H_

#include "Core/Customer.h"
#include <vector>
#include <optional>

/**
 * Abstract interface for customer and cashback operations.
 */

/**
 * Create a new customer with the provided name, surname, and cashback strategy.
 * @param name Customer's first name.
 * @param surname Customer's surname.
 * @param cashback_strategy Strategy used to calculate the customer's cashback.
 * @returns New customer's identifier.
 */
 
/**
 * Retrieve a customer by identifier.
 * @param customer_id Identifier of the customer to retrieve.
 * @returns `std::optional<Customer>` containing the customer if found, empty otherwise.
 */

/**
 * Return all customers.
 * @returns A vector containing every Customer.
 */

/**
 * Update the name of the specified customer.
 * @param customer_id Identifier of the customer to update.
 * @param name New first name for the customer.
 */

/**
 * Update the surname of the specified customer.
 * @param customer_id Identifier of the customer to update.
 * @param surname New surname for the customer.
 */

/**
 * Apply cashback usage for the specified customer, reducing their available cashback by the given amount.
 * @param customer_id Identifier of the customer using cashback.
 * @param amount Amount of cashback to apply.
 */

/**
 * Add cashback to the specified customer based on a receipt total.
 * @param customer_id Identifier of the customer to credit.
 * @param receipt_total Total value of the receipt used to calculate cashback.
 */
class ICashbackService
{
public:
    virtual ~ICashbackService() = default;

    virtual int CreateCustomer(std::string name, std::string surname, std::shared_ptr<ICashbackStrategy> cashback_strategy) = 0;
    virtual std::optional<Customer> GetCustomer(int customer_id) = 0;
    virtual std::vector<Customer> GetAllCustomers() = 0;
    virtual void SetCustomerName(int customer_id, std::string name) = 0;
    virtual void SetCustomerSurname(int customer_id, std::string surname) = 0;
    // virtual void SetCustomerCashbackStrategy(std::shared_ptr<ICashbackStrategy> strategy) = 0;
    virtual void UseCashback(int customer_id, double amount) = 0;
    virtual void AddCashback(int customer_id, double receipt_total) = 0;
};

#endif // SRC_INFRASTRUCTURE_ICASHBACKSERVICE_H_