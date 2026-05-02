#ifndef SRC_CONSOLE_IVIEW_H_
#define SRC_CONSOLE_IVIEW_H_

#include "Bussiness/ProductService.h"
#include "Bussiness/DiscountService.h"
#include "Bussiness/ReceiptService.h"
#include <memory>

struct Context
{
    std::shared_ptr<ProductService> product_service;
    std::shared_ptr<DiscountService> discount_service;
    std::shared_ptr<ReceiptService> receipt_service;
};

class IView
{
public:
    virtual ~IView() = default;
    virtual std::unique_ptr<IView> Run() = 0;
};

#endif // SRC_CONSOLE_IVIEW_H_