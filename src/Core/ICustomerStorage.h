#ifndef SRC_CORE_ICUSTOMERSTORAGE_H_
#define SRC_CORE_ICUSTOMERSTORAGE_H_

#include "Core/Customer.h"
#include <functional>
#include <vector>

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