#include "Infrastructure/JSONReceiptStorage.h"
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

        for(const auto& r : j["receipts"])
        {
            std::vector<ReceiptItem> items;
            for(const auto& i : r["items"])
            {
                const auto& p = i["product"];
                Product product(
                    p["id"].get<int>(),
                    p["name"].get<std::string>(),
                    p["price"].get<double>(),
                    nullptr
                );
                int quantity = i["quantity"].get<int>();
                items.push_back(ReceiptItem(product, quantity));
            }

            Receipt receipt(
                r["id"].get<int>(),
                r["timestamp"].get<time_t>(),
                items,
                r["status"].get<ReceiptStatus>()
            );

            if(receipt.GetID() >= m_NextID) m_NextID = receipt.GetID() + 1;

            m_Receipts.emplace(receipt.GetID(), receipt);
        }

        file.close();
    }
}

JSONReceiptStorage::~JSONReceiptStorage()
{
    nlohmann::json j;
    j["receipts"] = nlohmann::json::array();

    for(const auto& [id, r] : m_Receipts)
    {
        nlohmann::json receipt_json;
        receipt_json["id"] = r.GetID();
        receipt_json["timestamp"] = r.GetTimestamp();
        receipt_json["status"] = r.GetStatus();
        receipt_json["items"] = nlohmann::json::array();

        for(const auto& i : r.GetItems())
        {
            const Product p = i.GetProduct();
            receipt_json["items"].push_back({
                {
                    "product",
                    {
                        {"id", p.GetID()},
                        {"name", p.GetName()},
                        {"price", p.GetPrice()}
                    }
                },
                {"quantity", i.GetQuantity()}
            });
        }

        j["receipts"].push_back(receipt_json);
    }

    std::ofstream file(m_Path);
    file << j.dump(4);
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

Receipt JSONReceiptStorage::GetReceipt(int id)
{
    auto it = m_Receipts.find(id);
    if(it == m_Receipts.end()) throw std::runtime_error("Receipt does not exist");
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