#ifndef SRC_INFRASTRUCTURE_JSONCUSTOMERSERIALIZER_H_
#define SRC_INFRASTRUCTURE_JSONCUSTOMERSERIALIZER_H_

#include "Core/Customer.h"
#include <nlohmann/json.hpp>

/**
 * Convert a Customer into its JSON representation.
 * @param customer Customer object to serialize.
 * @returns A nlohmann::json containing the customer's data.
 */

/**
 * Construct a Customer from its JSON representation.
 * @param customer_json JSON value containing the customer's data.
 * @returns A Customer built from the provided JSON.
 */
class JSONCustomerSerializer
{
public:
    nlohmann::json Serialize(Customer customer);
    Customer Deserialize(nlohmann::json customer_json);
};

#endif // SRC_INFRASTRUCTURE_JSONCUSTOMERSERIALIZER_H_