#include "Infrastructure/JSONReceiptStorage.h"
#include "Infrastructure/JSONReceiptSerializer.h"
#include <nlohmann/json.hpp>
#include <fstream>

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

int JSONReceiptStorage::AddReceipt(Receipt receipt)
{
    int id = m_NextID++;
    receipt.SetID(id);
    m_Receipts.emplace(id, receipt);
    return id;
}

void JSONReceiptStorage::UpdateReceipt(Receipt receipt)
{
    auto it = m_Receipts.find(receipt.GetID());
    if(it == m_Receipts.end()) throw std::runtime_error("Receipt does not exist");
    it->second = receipt;
}

std::optional<Receipt> JSONReceiptStorage::GetReceipt(int id)
{
    auto it = m_Receipts.find(id);
    if(it == m_Receipts.end()) std::nullopt;
    return it->second;
}

std::vector<Receipt> JSONReceiptStorage::GetReceipts(std::function<bool(Receipt)> predicate)
{
    std::vector<Receipt> receipts;
    for(auto& [id, receipt] : m_Receipts)
    {
        if(predicate(receipt)) receipts.push_back(receipt);
    }
    return receipts;
}

std::vector<Receipt> JSONReceiptStorage::GetAllReceipts()
{
    std::vector<Receipt> receipts;
    for(auto& receipt : m_Receipts) receipts.push_back(receipt.second);
    return receipts;
}

void JSONReceiptStorage::RemoveReceipt(int id)
{
    m_Receipts.erase(id);
}