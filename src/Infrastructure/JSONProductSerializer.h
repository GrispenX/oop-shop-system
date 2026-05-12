#ifndef SRC_INFRASTRUCTURE_JSONPRODUCTSERIALIZATION_H_
#define SRC_INFRASTRUCTURE_JSONPRODUCTSERIALIZATION_H_

#include "Core/Product.h"
#include "Bussiness/RegularDiscount.h"
#include "Bussiness/BundleDiscount.h"
#include <nlohmann/json.hpp>

/**
 * Convert a Product into a JSON representation.
 *
 * @param product Product to serialize.
 * @returns A `nlohmann::json` object representing the product, including identifier, name, pricing, inventory, and any discount information.
 */

/**
 * Construct a Product from its JSON representation.
 *
 * @param product_json JSON object produced by `Serialize` (expected to contain identifier, name, price, inventory, and discount fields).
 * @returns A `Product` populated from the provided JSON.
 */
class JSONProductSerializer
{
public:
    nlohmann::json Serialize(Product product);
    Product Deserialize(nlohmann::json product_json);
};

#endif // SRC_INFRASTRUCTURE_JSONPRODUCTSERIALIZATION_H_