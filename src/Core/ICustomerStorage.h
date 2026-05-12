#ifndef SRC_CORE_ICUSTOMERSTORAGE_H_
#define SRC_CORE_ICUSTOMERSTORAGE_H_

#include "Core/Customer.h"
#include <functional>
#include <vector>

/**
 * Abstract interface for persistent storage and retrieval of Customer records.
 *
 * Implementations provide creation, update, retrieval (by id, by predicate, or all),
 * and removal of Customer entries.
 */
 
/**
 * Add a new customer to storage and return its assigned identifier.
 * @param customer Customer to add; ownership/copying semantics depend on implementation.
 * @returns Assigned integer identifier for the newly added customer.
 */

/**
 * Update an existing customer record in storage.
 * @param customer Customer containing updated fields; the implementation identifies which record to update (e.g., by id).
 */

/**
 * Retrieve a customer by its identifier.
 * @param id Identifier of the customer to retrieve.
 * @returns The Customer associated with the provided id.
 */

/**
 * Retrieve all customers that satisfy the given predicate.
 * @param predicate Function invoked for each Customer; customers for which the predicate returns `true` are included in the result.
 * @returns A vector of Customer objects that match the predicate.
 */

/**
 * Retrieve all customers from storage.
 * @returns A vector containing all stored Customer objects.
 */

/**
 * Remove the customer with the specified identifier from storage.
 * @param id Identifier of the customer to remove.
 */
class ICustomerStorage
{
public:
    virtual ~ICustomerStorage() = default;

    virtual int Add(Customer customer) = 0;
    virtual void Update(Customer customer) = 0;
    virtual Customer Get(int id) = 0;
    virtual std::vector<Customer> Get(std::function<bool(Customer)> predicate) = 0;
    virtual std::vector<Customer> GetAll() = 0;
    virtual void Remove(int id) = 0;
};

#endif // SRC_CORE_ICUSTOMERSTORAGE_H_