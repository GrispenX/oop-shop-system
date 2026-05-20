#include "Infrastructure/JSONCustomerSerializer.h"
#include "Bussiness/RegularCashback.h"

nlohmann::json JSONCustomerSerializer::Serialize(Customer customer)
{
    nlohmann::json cashback_strat_json;
    std::shared_ptr<ICashbackStrategy> cashback_strat = customer.GetCashbackStrategy();
    if(auto regular = std::dynamic_pointer_cast<RegularCashback>(cashback_strat))
    {
        cashback_strat_json["type"] = "regular";
    }
    else
    {
        throw std::runtime_error("Unknown cashback strategy type");
    }

    return nlohmann::json {
        {"id", customer.GetID()},
        {"name", customer.GetName()},
        {"surname", customer.GetSurname()},
        {"cashback_balance", customer.GetBalance()},
        {"cashback_strategy", cashback_strat_json}
    };
}

Customer JSONCustomerSerializer::Deserialize(nlohmann::json customer_json)
{
    if(!(
        customer_json.contains("id") &&
        customer_json.contains("name") &&
        customer_json.contains("surname") &&
        customer_json.contains("cashback_balance") &&
        customer_json.contains("cashback_strategy")
    )) throw std::runtime_error("Invalid json");

    int id = customer_json["id"].get<int>();
    std::string name = customer_json["name"].get<std::string>();
    std::string surname = customer_json["surname"].get<std::string>();
    double balance = customer_json["cashback_balance"].get<double>();
    std::shared_ptr<ICashbackStrategy> cashback_strat;

    nlohmann::json cashback_strat_json = customer_json["cashback_strategy"];
    std::string cashback_type = cashback_strat_json["type"].get<std::string>();
    if(cashback_type == "regular") cashback_strat = std::make_shared<RegularCashback>();
    else throw std::runtime_error("Unknown cashback strategy type");

    return Customer(id, name, surname, balance, cashback_strat);
}