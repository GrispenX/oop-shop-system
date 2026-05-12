#ifndef SRC_CONSOLE_IVIEW_H_
#define SRC_CONSOLE_IVIEW_H_

#include "Infrastructure/IProductService.h"
#include "Infrastructure/IReceiptService.h"
#include "Infrastructure/ICashbackService.h"
#include <memory>

struct Context
{
    std::shared_ptr<IProductService> product_service;
    std::shared_ptr<IReceiptService> receipt_service;
    std::shared_ptr<ICashbackService> cashback_service;
};

/**
 * Execute the view's interaction and return the next view to run.
 *
 * @returns std::unique_ptr<IView> owning the next view to execute; `nullptr` to indicate no further view (exit).
 */
class IView
{
public:
    virtual ~IView() = default;
    virtual std::unique_ptr<IView> Run() = 0;
};

#endif // SRC_CONSOLE_IVIEW_H_