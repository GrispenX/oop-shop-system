#include "Infrastructure/JSONCustomerStorage.h"
#include "Infrastructure/JSONCustomerSerializer.h"
#include <fstream>

/**
 * @brief Constructs a JSON-backed customer storage using the given filesystem path.
 *
 * If the file at `path` exists, loads stored customers from it and initializes internal
 * state so subsequent Add calls assign IDs greater than any loaded customer ID.
 *
 * @param path Filesystem path to the JSON storage file. If the file exists it will be read to populate stored customers; if it does not exist the storage starts empty and the file will be created on destruction.
 */
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

/**
 * @brief Serializes all stored customers and persists them to the configured file.
 *
 * Builds a JSON array of serialized customers and writes the pretty-printed
 * JSON document to m_Path when the storage is destroyed.
 */
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

/**
 * @brief Assigns a new unique ID to the provided customer and stores it.
 *
 * The customer is given the next available ID and inserted into the internal
 * storage map.
 *
 * @param customer Customer value to assign an ID and persist.
 * @return int Assigned customer ID.
 */
int JSONCustomerStorage::Add(Customer customer)
{
    int id = m_NextID++;
    customer.SetID(id);
    m_Customers.emplace(id, customer);
    return id;
}

/**
 * @brief Replace the stored customer entry with the provided customer.
 *
 * Replaces the customer record identified by customer.GetID() with the given
 * Customer object.
 *
 * @param customer Customer instance whose ID identifies the record to replace.
 *                 The provided object becomes the new stored value for that ID.
 * @throws std::runtime_error If no customer with the given ID exists.
 */
void JSONCustomerStorage::Update(Customer customer)
{
    auto it = m_Customers.find(customer.GetID());
    if(it == m_Customers.end()) throw std::runtime_error("Customer does not exist");
    it->second = customer;
}

/**
 * @brief Retrieves a copy of the customer with the given ID.
 *
 * @param id Customer identifier to look up.
 * @return Customer Copy of the stored customer with the specified ID.
 * @throws std::runtime_error if no customer exists with the specified ID.
 */
Customer JSONCustomerStorage::Get(int id)
{
    auto it = m_Customers.find(id);
    if(it == m_Customers.end()) throw std::runtime_error("Customer does not exist");
    return it->second;
}

/**
 * @brief Retrieves customers that satisfy the provided predicate.
 *
 * Applies the predicate to each stored customer and collects those for which
 * the predicate returns `true`.
 *
 * @param predicate Function invoked for each customer; customers for which it
 *                  returns `true` are included in the result.
 * @return std::vector<Customer> A vector of copies of customers that match the predicate.
 */
std::vector<Customer> JSONCustomerStorage::Get(std::function<bool(Customer)> predicate)
{
    std::vector<Customer> customers;
    for(auto& [id, customer] : m_Customers)
    {
        if(predicate(customer)) customers.push_back(customer);
    }
    return customers;
}

/**
 * @brief Retrieve all customers stored in memory.
 *
 * @return std::vector<Customer> A vector containing copies of every stored Customer.
 */
std::vector<Customer> JSONCustomerStorage::GetAll()
{
    std::vector<Customer> customers;
    for(auto& [id, customer] : m_Customers)
    {
        customers.push_back(customer);
    }
    return customers;
}

/**
 * @brief Removes the customer with the specified ID from storage.
 *
 * If no customer exists with the given ID, the storage remains unchanged.
 *
 * @param id The identifier of the customer to remove.
 */
void JSONCustomerStorage::Remove(int id)
{
    m_Customers.erase(id);
}