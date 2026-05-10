#include "Infrastructure/JSONProductStorage.h"
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

        for(const auto& p : j["products"])
        {
            Product product(
                p["id"].get<int>(),
                p["name"].get<std::string>(),
                p["price"].get<double>(),
                nullptr
            );
            if(product.GetID() >= m_NextID) m_NextID = product.GetID() + 1;
            m_Products.emplace(product.GetID(), product);
        }

        file.close();
    }
}

JSONProductStorage::~JSONProductStorage()
{
    nlohmann::json j;
    j["products"] = nlohmann::json::array();

    for(const auto& [id, p] : m_Products)
    {
        j["products"].push_back({
            {"id", p.GetID()},
            {"name", p.GetName()},
            {"price", p.GetPrice()}
        });
    }

    std::ofstream file(m_Path);
    file << j.dump(4);
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