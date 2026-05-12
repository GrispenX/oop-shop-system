#include "Infrastructure/JSONReceiptStorage.h"
#include "Infrastructure/JSONReceiptSerializer.h"
#include <nlohmann/json.hpp>
#include <fstream>

/**
 * @brief Constructs a JSON-backed receipt storage and loads existing receipts if present.
 *
 * Initializes the storage path and next ID (starting at 1). If a file exists at the provided
 * path, loads receipts from the file, deserializes them, populates the in-memory receipt map,
 * and updates the next ID to one greater than the maximum loaded receipt ID.
 *
 * @param path Filesystem path to the JSON file used for persistent storage.
 */
JSONReceiptStorage::JSONReceiptStorage(std::filesystem::path path) :
    m_Path(path),
    m_NextID(1)
{
    if(std::filesystem::exists(path))
    {
        std::ifstream file(path);
        nlohmann::json j = nlohmann::json::parse(file);

        JSONReceiptSerializer serializer;

        for(const auto& receipt_json : j["receipts"])
        {
            Receipt receipt = serializer.Deserialize(receipt_json);
            int id = receipt.GetID();
            if(id >= m_NextID) m_NextID = id + 1;
            m_Receipts.emplace(id, receipt);
        }

        file.close();
    }
}

/**
 * @brief Persists all in-memory receipts to the configured JSON file path.
 *
 * @details Serializes the stored receipts into a JSON object with a top-level
 * "receipts" array and writes the pretty-printed JSON to the storage path,
 * overwriting any existing file.
 */
JSONReceiptStorage::~JSONReceiptStorage()
{
    nlohmann::json j;
    j["receipts"] = nlohmann::json::array();

    JSONReceiptSerializer serializer;

    for(const auto& [id, receipt] : m_Receipts)
    {
        nlohmann::json receipt_json = serializer.Serialize(receipt);
        j["receipts"].push_back(receipt_json);
    }

    std::ofstream file(m_Path);
    file << j.dump(4);
    file.close();
}

/**
 * @brief Assigns a new unique ID to a receipt and stores it in the in-memory collection.
 *
 * The provided receipt has its ID updated to the newly assigned value and a copy is inserted into storage.
 *
 * @param receipt Receipt object to store; its ID will be set to the assigned value.
 * @return int Assigned receipt ID.
 */
int JSONReceiptStorage::AddReceipt(Receipt receipt)
{
    int id = m_NextID++;
    receipt.SetID(id);
    m_Receipts.emplace(id, receipt);
    return id;
}

/**
 * @brief Replaces the stored receipt that has the same ID as the provided receipt.
 *
 * The provided receipt's ID is used to locate the existing entry; the stored value is
 * replaced with the provided receipt.
 *
 * @param receipt Receipt whose ID identifies the entry to replace.
 * @throws std::runtime_error if no receipt with the provided ID exists.
 */
void JSONReceiptStorage::UpdateReceipt(Receipt receipt)
{
    auto it = m_Receipts.find(receipt.GetID());
    if(it == m_Receipts.end()) throw std::runtime_error("Receipt does not exist");
    it->second = receipt;
}

/**
 * @brief Retrieve a stored receipt by its ID.
 *
 * @param id ID of the receipt to retrieve.
 * @return Receipt The receipt matching the provided id.
 * @throws std::runtime_error if no receipt exists with the given id.
 */
Receipt JSONReceiptStorage::GetReceipt(int id)
{
    auto it = m_Receipts.find(id);
    if(it == m_Receipts.end()) throw std::runtime_error("Receipt does not exist");
    return it->second;
}

/**
 * @brief Returns receipts that satisfy a filter predicate.
 *
 * @param predicate Callable invoked for each stored Receipt; if it returns `true`, that receipt is included in the result. The predicate receives a copy of each stored receipt.
 * @return std::vector<Receipt> Vector of copies of receipts for which `predicate` returned `true`.
 */
std::vector<Receipt> JSONReceiptStorage::GetReceipts(std::function<bool(Receipt)> predicate)
{
    std::vector<Receipt> receipts;
    for(auto& [id, receipt] : m_Receipts)
    {
        if(predicate(receipt)) receipts.push_back(receipt);
    }
    return receipts;
}

/**
 * @brief Retrieves all stored receipts.
 *
 * The returned vector contains a copy of every receipt currently held by the storage.
 * The order of receipts is unspecified.
 *
 * @return std::vector<Receipt> Vector of all stored receipts.
 */
std::vector<Receipt> JSONReceiptStorage::GetAllReceipts()
{
    std::vector<Receipt> receipts;
    for(auto& receipt : m_Receipts) receipts.push_back(receipt.second);
    return receipts;
}

/**
 * @brief Removes the receipt with the specified ID from storage.
 *
 * If no receipt exists with the given ID, the function has no effect.
 *
 * @param id ID of the receipt to remove.
 */
void JSONReceiptStorage::RemoveReceipt(int id)
{
    m_Receipts.erase(id);
}