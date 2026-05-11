#ifndef SRC_CONSOLE_IVIEW_H_
#define SRC_CONSOLE_IVIEW_H_

#include "Infrastructure/IProductService.h"
#include "Infrastructure/IReceiptService.h"
#include <memory>

struct Context
{
    std::shared_ptr<IProductService> product_service;
    std::shared_ptr<IReceiptService> receipt_service;
};

class IView
{
public:
    virtual ~IView() = default;
    virtual std::unique_ptr<IView> Run() = 0;
};

#endif // SRC_CONSOLE_IVIEW_H_