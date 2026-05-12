#ifndef SRC_INFRASTRUCTURE_JSONRECEIPTSERIALIZER_H_
#define SRC_INFRASTRUCTURE_JSONRECEIPTSERIALIZER_H_

#include "Core/Receipt.h"
#include "Infrastructure/JSONProductSerializer.h"
#include <nlohmann/json.hpp>

/**
 * Convert a Receipt to a JSON representation.
 * @param receipt Receipt instance to serialize.
 * @returns nlohmann::json object representing the provided receipt.
 */
/**
 * Construct a Receipt from its JSON representation.
 * @param receipt_json JSON object containing receipt data.
 * @returns Receipt constructed from the provided JSON.
 */
class JSONReceiptSerializer
{
public:
    nlohmann::json Serialize(Receipt receipt);
    Receipt Deserialize(nlohmann::json receipt_json);
};

#endif // SRC_INFRASTRUCTURE_JSONRECEIPTSERIALIZER_H_