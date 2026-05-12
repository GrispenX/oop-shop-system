#ifndef SRC_INFRASTRUCTURE_JSONCUSTOMERSTORAGE_H_
#define SRC_INFRASTRUCTURE_JSONCUSTOMERSTORAGE_H_

#include "Core/ICustomerStorage.h"
#include <unordered_map>
#include <filesystem>

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