#ifndef SRC_BUSSINESS_DISCOUNTSERVICE_H_
#define SRC_BUSSINESS_DISCOUNTSERVICE_H_

#include "Core/IProductStorage.h"
#include <memory>
#include <vector>

class DiscountService
{
public:
    DiscountService(std::shared_ptr<IProductStorage> product_storage);

    void SetDiscount(int product_id, std::shared_ptr<IDiscountStrategy> discount);
    std::vector<Product> GetDiscountedProducts();

private:
    std::shared_ptr<IProductStorage> m_ProductStorage;
};

#endif // SRC_BUSSINESS_DISCOUNTSERVICE_H_