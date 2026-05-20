#include "Infrastructure/JSONProductSerializer.h"

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

Product JSONProductSerializer::Deserialize(nlohmann::json product_json)
{
    if(!(
        product_json.contains("id") &&
        product_json.contains("name") &&
        product_json.contains("price") &&
        product_json.contains("discount") &&
        product_json["discount"].contains("type")
    )) throw std::runtime_error("Invalid json");

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