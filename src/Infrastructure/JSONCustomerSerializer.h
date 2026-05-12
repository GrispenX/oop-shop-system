#ifndef SRC_INFRASTRUCTURE_JSONCUSTOMERSERIALIZER_H_
#define SRC_INFRASTRUCTURE_JSONCUSTOMERSERIALIZER_H_

#include "Core/Customer.h"
#include <nlohmann/json.hpp>

class JSONCustomerSerializer
{
public:
    nlohmann::json Serialize(Customer customer);
    Customer Deserialize(nlohmann::json customer_json);
};

#endif // SRC_INFRASTRUCTURE_JSONCUSTOMERSERIALIZER_H_