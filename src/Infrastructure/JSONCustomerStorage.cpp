#include "Infrastructure/JSONCustomerStorage.h"
#include "Infrastructure/JSONCustomerSerializer.h"
#include <fstream>

JSONCustomerStorage::JSONCustomerStorage(std::filesystem::path path) :
    m_Path(path)
{
    if(std::filesystem::exists(path))
    {
        std::ifstream file(path);
        nlohmann::json j = nlohmann::json::parse(file);
        
        JSONCustomerSerializer serializer;

        for(const auto& customer_json : j["customers"])
        {
            Customer customer = serializer.Deserialize(customer_json);
            int id = customer.GetID();
            if(m_NextID <= id) m_NextID = id + 1;
            m_Customers.emplace(id, customer);
        }

        file.close();
    }
}

JSONCustomerStorage::~JSONCustomerStorage()
{
    nlohmann::json j;
    j["customers"] = nlohmann::json::array();

    JSONCustomerSerializer serializer;

    for(const auto& [id, customer] : m_Customers)
    {
        nlohmann::json customer_json = serializer.Serialize(customer);
        j["customers"].push_back(customer_json);
    }

    std::ofstream file(m_Path);
    file << j.dump(4);
    file.close();
}

int JSONCustomerStorage::Add(Customer customer)
{
    int id = m_NextID++;
    customer.SetID(id);
    m_Customers.emplace(id, customer);
    return id;
}

void JSONCustomerStorage::Update(Customer customer)
{
    auto it = m_Customers.find(customer.GetID());
    if(it == m_Customers.end()) throw std::runtime_error("Customer does not exist");
    it->second = customer;
}

std::optional<Customer> JSONCustomerStorage::Get(int id)
{
    auto it = m_Customers.find(id);
    if(it == m_Customers.end()) return std::nullopt;
    return it->second;
}

std::vector<Customer> JSONCustomerStorage::Get(std::function<bool(Customer)> predicate)
{
    std::vector<Customer> customers;
    for(auto& [id, customer] : m_Customers)
    {
        if(predicate(customer)) customers.push_back(customer);
    }
    return customers;
}

std::vector<Customer> JSONCustomerStorage::GetAll()
{
    std::vector<Customer> customers;
    for(auto& [id, customer] : m_Customers)
    {
        customers.push_back(customer);
    }
    return customers;
}

void JSONCustomerStorage::Remove(int id)
{
    m_Customers.erase(id);
}