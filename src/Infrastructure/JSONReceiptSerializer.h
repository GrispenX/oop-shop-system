#ifndef SRC_INFRASTRUCTURE_JSONRECEIPTSERIALIZER_H_
#define SRC_INFRASTRUCTURE_JSONRECEIPTSERIALIZER_H_

#include "Core/Receipt.h"
#include "Infrastructure/JSONProductSerializer.h"
#include <nlohmann/json.hpp>

class JSONReceiptSerializer
{
public:
    nlohmann::json Serialize(Receipt receipt);
    Receipt Deserialize(nlohmann::json receipt_json);
};

#endif // SRC_INFRASTRUCTURE_JSONRECEIPTSERIALIZER_H_