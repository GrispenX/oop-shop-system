#ifndef SRC_INFRASTRUCTURE_JSONPRODUCTSERIALIZATION_H_
#define SRC_INFRASTRUCTURE_JSONPRODUCTSERIALIZATION_H_

#include "Core/Product.h"
#include "Bussiness/RegularDiscount.h"
#include "Bussiness/BundleDiscount.h"
#include <nlohmann/json.hpp>

class JSONProductSerializer
{
public:
    nlohmann::json Serialize(Product product);
    Product Deserialize(nlohmann::json product_json);
};

#endif // SRC_INFRASTRUCTURE_JSONPRODUCTSERIALIZATION_H_