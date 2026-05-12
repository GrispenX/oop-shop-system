#include "Infrastructure/JSONProductStorage.h"
#include "Infrastructure/JSONProductSerializer.h"
#include <nlohmann/json.hpp>
#include <fstream>

/**
 * @brief Constructs a JSON-backed product storage and initializes state from disk.
 *
 * If the specified path exists, loads stored products from the JSON file, inserts them
 * into the internal product map, and advances the next ID to one greater than the
 * maximum ID found so new products receive unique IDs.
 *
 * @param path Filesystem path to the JSON storage file.
 */
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

/**
 * @brief Persists all stored products to the instance's JSON file when the storage is destroyed.
 *
 * Serializes each product into a JSON array under the "products" key and writes the pretty-printed
 * JSON document to the path provided to the storage instance.
 */
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

/**
 * @brief Add a new product to storage and assign it a unique ID.
 *
 * The provided product's ID is set to the assigned value before being stored.
 *
 * @param product Product to store; its ID will be updated to the assigned ID.
 * @return int The assigned product ID.
 */
int JSONProductStorage::AddProduct(Product product)
{
    int id = m_NextID++;
    product.SetID(id);
    m_Products.emplace(id, product);
    return id;
}

/**
 * @brief Replace the stored product that matches the provided product's ID.
 *
 * The product's ID is used to locate the existing entry; the stored product is replaced
 * with the provided product.
 *
 * @param product Product whose ID determines which stored product will be replaced.
 * @throws std::runtime_error if no product with the given ID exists in storage.
 */
void JSONProductStorage::UpdateProduct(Product product)
{
    auto it = m_Products.find(product.GetID());
    if(it == m_Products.end()) throw std::runtime_error("Product does not exist");
    it->second = product;
}

/**
 * @brief Retrieve a product by its ID.
 *
 * @param id Product identifier to look up.
 * @return Product The product with the specified ID.
 * @throws std::runtime_error if no product exists with the given ID.
 */
Product JSONProductStorage::GetProduct(int id)
{
    auto it = m_Products.find(id);
    if(it == m_Products.end()) throw std::runtime_error("Product does not exist");
    return it->second;
}

/**
 * @brief Filters stored products using a predicate and returns the matching entries.
 *
 * @param predicate Function invoked for each product; products for which it returns `true` are included in the result.
 * @return std::vector<Product> Vector of products that satisfy `predicate`.
 */
std::vector<Product> JSONProductStorage::GetProducts(std::function<bool(Product)> predicate)
{
    std::vector<Product> products;
    for(auto& [id, product] : m_Products)
    {
        if(predicate(product)) products.push_back(product);
    }
    return products;
}

/**
 * @brief Retrieves all products stored in this storage.
 *
 * @return std::vector<Product> A vector of copies of every stored Product (order unspecified).
 */
std::vector<Product> JSONProductStorage::GetAllProducts()
{
    std::vector<Product> products;
    for(auto& product : m_Products) products.push_back(product.second);
    return products;
}

/**
 * @brief Removes the product with the given ID from storage.
 *
 * If no product exists with the specified ID the call is a no-op.
 *
 * @param id ID of the product to remove.
 */
void JSONProductStorage::RemoveProduct(int id)
{
    m_Products.erase(id);
}