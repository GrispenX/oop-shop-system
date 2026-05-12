#include "Infrastructure/JSONProductStorage.h"
#include "Infrastructure/JSONReceiptStorage.h"
#include "Infrastructure/JSONInventoryStorage.h"
#include "Infrastructure/JSONCustomerStorage.h"
#include "Infrastructure/ProductService.h"
#include "Infrastructure/ReceiptService.h"
#include "Infrastructure/CashbackService.h"
#include "Console/MainView.h"

/**
 * @brief Initialize storage and service dependencies, build the application context, and run the UI loop.
 *
 * The program constructs JSON-backed storages and service instances, assembles them into a Context,
 * creates the initial MainView, and repeatedly runs view transitions until the UI loop ends.
 *
 * @return int Process exit code: `0` on normal termination.
 */
int main()
{
    std::shared_ptr<IProductStorage> product_storage = std::make_shared<JSONProductStorage>("products.json");
    std::shared_ptr<IReceiptStorage> receipt_storage = std::make_shared<JSONReceiptStorage>("receipts.json");
    std::shared_ptr<IInventoryStorage> inventory_storage = std::make_shared<JSONInventoryStorage>("inventory.json");
    std::shared_ptr<ICustomerStorage> customer_storage = std::make_shared<JSONCustomerStorage>("customers.json");

    std::shared_ptr<IProductService> product_service = std::make_shared<ProductService>(product_storage, inventory_storage);
    std::shared_ptr<ICashbackService> cashback_service = std::make_shared<CashbackService>(customer_storage);
    std::shared_ptr<IReceiptService> receipt_service = std::make_shared<ReceiptService>(product_storage, receipt_storage, product_service, cashback_service);

    Context context {
        .product_service = product_service,
        .receipt_service = receipt_service,
        .cashback_service = cashback_service
    };

    std::unique_ptr<IView> view = std::make_unique<MainView>(context);
    while(view)
    {
        view = std::move(view->Run());
    }
}