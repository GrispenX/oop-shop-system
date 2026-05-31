add_library(Console
    TerminalStyle.cpp

    MainView.cpp
    ProductManagement/ProductsView.cpp
    ProductManagement/ListProductsView.cpp
    ProductManagement/AddProductView.cpp
    ProductManagement/SelectProductView.cpp
    ProductManagement/EditProductView.cpp

    ReceiptManagement/ReceiptsView.cpp
    ReceiptManagement/ListReceiptsView.cpp
    ReceiptManagement/SelectReceiptView.cpp
    ReceiptManagement/ReceiptDetailsView.cpp

    CashbackManagement/CashbackView.cpp
    CashbackManagement/ListCustomersView.cpp
    CashbackManagement/EditCustomerView.cpp
)

target_include_directories(Console PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Console PUBLIC Core Bussiness Infrastructure)