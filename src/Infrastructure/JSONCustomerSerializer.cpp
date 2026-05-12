#include "Infrastructure/JSONCustomerSerializer.h"
#include "Bussiness/RegularCashback.h"

/**
 * @brief Serialize a Customer into a JSON representation including its cashback strategy.
 *
 * The produced JSON contains the customer's id, name, surname, cashback_balance, and a
 * cashback_strategy object that encodes the strategy type.
 *
 * @return nlohmann::json JSON object with keys: `id`, `name`, `surname`, `cashback_balance`, and `cashback_strategy`.
 * @throws std::runtime_error if the customer's cashback strategy type is not recognized (only `"regular"` is supported).
 */
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

/**
 * @brief Constructs a Customer from a JSON object and restores its cashback strategy.
 *
 * @param customer_json JSON object containing keys:
 *   - "id" (int)
 *   - "name" (string)
 *   - "surname" (string)
 *   - "cashback_balance" (double)
 *   - "cashback_strategy" (object) with "type" (string), currently supports "regular".
 * @return Customer Instance populated from the JSON data.
 * @throws std::runtime_error If the cashback strategy `type` is not recognized.
 */
Customer JSONCustomerSerializer::Deserialize(nlohmann::json customer_json)
{
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