#include "Infrastructure/JSONReceiptSerializer.h"

#include <iostream>

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

    nlohmann::json receipt_json = {
        {"id", receipt.GetID()},
        {"timestamp", receipt.GetTimestamp()},
        {"status", receipt.GetStatus()},
        {"items", items_json}
    };

    std::optional<int> customer_id = receipt.GetCustomerID();
    if(customer_id.has_value())
    {
        receipt_json["customer_id"] = customer_id.value();
        receipt_json["used_cashback"] = receipt.GetUsedCashback();
    }

    return receipt_json;
}

Receipt JSONReceiptSerializer::Deserialize(nlohmann::json receipt_json)
{
    if(!(
        receipt_json.contains("id") &&
        receipt_json.contains("timestamp") &&
        receipt_json.contains("status") &&
        receipt_json.contains("items")
    )) throw std::runtime_error("Invalid json");

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
    std::optional<int> customer_id = std::nullopt;
    double used_cashback = 0;

    if(receipt_json.contains("customer_id") && receipt_json.contains("used_cashback"))
    {
        customer_id = receipt_json["customer_id"].get<int>();
        used_cashback = receipt_json["used_cashback"].get<double>();
    }

    return Receipt(id, timestamp, items, status, customer_id, used_cashback);
}