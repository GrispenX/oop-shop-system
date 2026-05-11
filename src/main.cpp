#include "Infrastructure/JSONProductStorage.h"
#include "Infrastructure/JSONReceiptStorage.h"
#include "Infrastructure/JSONInventoryStorage.h"
#include "Infrastructure/ProductService.h"
#include "Infrastructure/ReceiptService.h"
#include "Console/MainView.h"

int main()
{
    std::shared_ptr<IProductStorage> product_storage = std::make_shared<JSONProductStorage>("products.json");
    std::shared_ptr<IReceiptStorage> receipt_storage = std::make_shared<JSONReceiptStorage>("receipts.json");
    std::shared_ptr<IInventoryStorage> inventory_storage = std::make_shared<JSONInventoryStorage>("inventory.json");

    Context context {
        .product_service = std::make_shared<ProductService>(product_storage, inventory_storage),
        .receipt_service = std::make_shared<ReceiptService>(product_storage, receipt_storage)
    };

    std::unique_ptr<IView> view = std::make_unique<MainView>(context);
    while(view)
    {
        view = std::move(view->Run());
    }
}