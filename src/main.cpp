#include "Infrastructure/JSONProductStorage.h"
#include "Infrastructure/JSONReceiptStorage.h"
#include "Infrastructure/JSONInventoryStorage.h"
#include "Infrastructure/JSONCustomerStorage.h"
#include "Infrastructure/ProductService.h"
#include "Infrastructure/ReceiptService.h"
#include "Infrastructure/CashbackService.h"
#include "Console/MainView.h"

int main()
{
    std::shared_ptr<IProductStorage> product_storage = std::make_shared<JSONProductStorage>("products.json");
    std::shared_ptr<IReceiptStorage> receipt_storage = std::make_shared<JSONReceiptStorage>("receipts.json");
    std::shared_ptr<IInventoryStorage> inventory_storage = std::make_shared<JSONInventoryStorage>("inventory.json");
    std::shared_ptr<ICustomerStorage> customer_storage = std::make_shared<JSONCustomerStorage>("customers.json");

    std::shared_ptr<IProductService> product_service = std::make_shared<ProductService>(product_storage, inventory_storage);
    std::shared_ptr<IReceiptService> receipt_service = std::make_shared<ReceiptService>(product_storage, receipt_storage, product_service);
    std::shared_ptr<ICashbackService> cashback_service = std::make_shared<CashbackService>(customer_storage);

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