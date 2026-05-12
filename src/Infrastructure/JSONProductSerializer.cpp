#include "Infrastructure/JSONProductSerializer.h"

/**
 * @brief Serialize a Product into a JSON representation that includes nested discount data.
 *
 * The resulting JSON contains the product fields "id", "name", and "price", and a nested
 * "discount" object whose "type" is one of "none", "regular", or "bundle". When "regular",
 * the discount object contains "percentage". When "bundle", it contains "min_quantity" and "percentage".
 *
 * @return nlohmann::json JSON object with keys: "id", "name", "price", and "discount".
 *
 * @throws std::runtime_error If the product's discount exists but is of an unrecognized runtime type.
 */
nlohmann::json JSONProductSerializer::Serialize(Product product)
{    
    nlohmann::json discount_json;
    std::shared_ptr<IDiscountStrategy> discount = product.GetDiscount();
    if(!discount)
    {
        discount_json["type"] = "none";
    }
    else if(auto regular = std::dynamic_pointer_cast<RegularDiscount>(discount))
    {
        discount_json["type"] = "regular";
        discount_json["percentage"] = regular->GetPercentage();
    }
    else if(auto bundle = std::dynamic_pointer_cast<BundleDiscount>(discount))
    {
        discount_json["type"] = "bundle";
        discount_json["min_quantity"] = bundle->GetMinQuantity();
        discount_json["percentage"] = bundle->GetPercentage();
    }
    else
    {
        throw std::runtime_error("Unknown discount type");
    }

    return nlohmann::json {
        {"id", product.GetID()},
        {"name", product.GetName()},
        {"price", product.GetPrice()},
        {"discount", discount_json}
    };
}

/**
 * Deserialize a Product from a JSON object.
 *
 * Expects product_json to contain:
 * - "id" (int), "name" (string), and "price" (double).
 * - A nested "discount" object with a "type" string:
 *   - "none" — no discount.
 *   - "regular" — requires "percentage" (double).
 *   - "bundle" — requires "min_quantity" (int) and "percentage" (double).
 *
 * @param product_json JSON object containing the product fields described above.
 * @return Product Constructed Product with the parsed id, name, price, and discount strategy.
 * @throws std::runtime_error If the discount "type" is unrecognized.
 */
Product JSONProductSerializer::Deserialize(nlohmann::json product_json)
{
    int id = product_json["id"].get<int>();
    std::string name = product_json["name"].get<std::string>();
    double price = product_json["price"].get<double>();
    std::shared_ptr<IDiscountStrategy> discount;

    nlohmann::json discount_json = product_json["discount"];

    std::string discount_type = discount_json["type"].get<std::string>();
    if(discount_type == "none") discount = nullptr;
    else if(discount_type == "regular") discount = std::make_shared<RegularDiscount>(discount_json["percentage"].get<double>());
    else if(discount_type == "bundle") discount = std::make_shared<BundleDiscount>(discount_json["min_quantity"].get<int>(), discount_json["percentage"].get<double>());
    else throw std::runtime_error("Unknown discount type");

    return Product(id, name, price, discount);
}