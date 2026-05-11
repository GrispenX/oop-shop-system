#include "Infrastructure/JSONReceiptSerializer.h"

nlohmann::json JSONReceiptSerializer::Serialize(Receipt receipt)
{
    JSONProductSerializer product_serializer;

    nlohmann::json items_json = nlohmann::json::array();
    for(const auto& item : receipt.GetItems())
    {
        items_json.push_back({
            {"product", product_serializer.Serialize(item.GetProduct())},
            {"quantity", item.GetQuantity()}
        });
    }

    return nlohmann::json {
        {"id", receipt.GetID()},
        {"timestamp", receipt.GetTimestamp()},
        {"status", receipt.GetStatus()},
        {"items", items_json}
    };
}

Receipt JSONReceiptSerializer::Deserialize(nlohmann::json receipt_json)
{
    JSONProductSerializer product_serializer;

    std::vector<ReceiptItem> items;
    for(const auto& item_json : receipt_json["items"])
    {
        Product product = product_serializer.Deserialize(item_json["product"]);
        int quantity = item_json["quantity"].get<int>();
        items.push_back(ReceiptItem(product, quantity));
    }

    int id = receipt_json["id"].get<int>();
    time_t timestamp = receipt_json["timestamp"].get<time_t>();
    ReceiptStatus status = receipt_json["status"].get<ReceiptStatus>();

    return Receipt(id, timestamp, items, status);
}