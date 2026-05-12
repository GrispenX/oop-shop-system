#include "Infrastructure/JSONReceiptSerializer.h"

#include <iostream>

/**
 * @brief Serialize a Receipt into a JSON object.
 *
 * The resulting JSON contains the receipt's `id`, `timestamp`, `status`, and an
 * `items` array where each element has `product` (product JSON) and `quantity`.
 * If the receipt has a customer ID, the JSON also includes `customer_id` and
 * `used_cashback`.
 *
 * @param receipt Receipt to serialize.
 * @return nlohmann::json JSON object representing the receipt.
 */
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

/**
 * @brief Reconstructs a Receipt from its JSON representation.
 *
 * Parses the given JSON to produce a Receipt with its id, timestamp, status,
 * and list of items (each with product and quantity). If the JSON contains
 * customer information, the resulting Receipt will include `customer_id` and
 * `used_cashback`; otherwise `customer_id` will be empty and `used_cashback`
 * will be 0.
 *
 * @param receipt_json JSON object representing a receipt.
 * @return Receipt The deserialized Receipt.
 */
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
    std::optional<int> customer_id = std::nullopt;
    double used_cashback = 0;

    if(receipt_json.contains("customer"))
    {
        customer_id = receipt_json["customer_id"].get<int>();
        used_cashback = receipt_json["used_cashback"].get<double>();
    }

    return Receipt(id, timestamp, items, status, customer_id, used_cashback);
}