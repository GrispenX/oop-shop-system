#ifndef SRC_INFRASTRUCTURE_JSONCUSTOMERSTORAGE_H_
#define SRC_INFRASTRUCTURE_JSONCUSTOMERSTORAGE_H_

#include "Core/ICustomerStorage.h"
#include <unordered_map>
#include <filesystem>

/**
 * Concrete ICustomerStorage implementation that persists customers to a JSON-backed filesystem path
 * and keeps an in-memory map of customers indexed by integer ID.
 *
 * @param path Filesystem path used for persistence (file or directory depending on implementation).
 */
/**
 * Add a new customer and assign it a unique integer identifier.
 *
 * @param customer Customer to add; the storage assigns the customer's ID.
 * @returns The integer ID assigned to the added customer.
 */
/**
 * Update an existing customer record in storage.
 *
 * @param customer Customer object containing the updated data; the object's ID identifies which record to update.
 */
/**
 * Retrieve a customer by its integer ID.
 *
 * @param id Identifier of the customer to retrieve.
 * @returns The Customer matching the given ID.
 */
/**
 * Retrieve customers that satisfy the given predicate.
 *
 * @param predicate Function invoked for each customer; customers for which the predicate returns `true` are included.
 * @returns A vector of customers that satisfy `predicate`.
 */
/**
 * Retrieve all customers currently stored in memory.
 *
 * @returns A vector containing all stored customers.
 */
/**
 * Remove a customer identified by the given ID from storage.
 *
 * @param id Identifier of the customer to remove.
 */
class JSONCustomerStorage : public ICustomerStorage
{
public:
    JSONCustomerStorage(std::filesystem::path path);
    ~JSONCustomerStorage();

    int Add(Customer customer) override;
    void Update(Customer customer) override;
    Customer Get(int id) override;
    std::vector<Customer> Get(std::function<bool(Customer)> predicate) override;
    std::vector<Customer> GetAll() override;
    void Remove(int id) override;

private:
    std::filesystem::path m_Path;
    std::unordered_map<int, Customer> m_Customers;
    int m_NextID;
};

#endif // SRC_INFRASTRUCTURE_JSONCUSTOMERSTORAGE_H_