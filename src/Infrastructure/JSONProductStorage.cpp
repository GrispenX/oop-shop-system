#include "Infrastructure/JSONProductStorage.h"
#include "Infrastructure/JSONProductSerializer.h"
#include <nlohmann/json.hpp>
#include <fstream>

JSONProductStorage::JSONProductStorage(std::filesystem::path path) :
    m_Path(path),
    m_NextID(1)
{
    if(std::filesystem::exists(path))
    {
        std::ifstream file(path);
        nlohmann::json j = nlohmann::json::parse(file);

        JSONProductSerializer serializer;

        for(const auto& product_json : j["products"])
        {
            Product product = serializer.Deserialize(product_json);
            int id = product.GetID();
            if(id >= m_NextID) m_NextID = id + 1;
            m_Products.emplace(id, product);
        }

        file.close();
    }
}

JSONProductStorage::~JSONProductStorage()
{
    nlohmann::json j;
    j["products"] = nlohmann::json::array();

    JSONProductSerializer serializer;

    for(const auto& [id, product] : m_Products)
    {
        nlohmann::json product_json = serializer.Serialize(product);
        j["products"].push_back(product_json);
    }

    std::ofstream file(m_Path);
    file << j.dump(4);
    file.close();
}

int JSONProductStorage::AddProduct(Product product)
{
    int id = m_NextID++;
    product.SetID(id);
    m_Products.emplace(id, product);
    return id;
}

void JSONProductStorage::UpdateProduct(Product product)
{
    auto it = m_Products.find(product.GetID());
    if(it == m_Products.end()) throw std::runtime_error("Product does not exist");
    it->second = product;
}

Product JSONProductStorage::GetProduct(int id)
{
    auto it = m_Products.find(id);
    if(it == m_Products.end()) throw std::runtime_error("Product does not exist");
    return it->second;
}

std::vector<Product> JSONProductStorage::GetProducts(std::function<bool(Product)> predicate)
{
    std::vector<Product> products;
    for(auto& [id, product] : m_Products)
    {
        if(predicate(product)) products.push_back(product);
    }
    return products;
}

std::vector<Product> JSONProductStorage::GetAllProducts()
{
    std::vector<Product> products;
    for(auto& product : m_Products) products.push_back(product.second);
    return products;
}

void JSONProductStorage::RemoveProduct(int id)
{
    m_Products.erase(id);
}