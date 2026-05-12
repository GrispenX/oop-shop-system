add_library(Infrastructure
    ProductService.cpp
    ReceiptService.cpp
    CashbackService.cpp
    JSONProductSerializer.cpp
    JSONReceiptSerializer.cpp
    JSONCustomerSerializer.cpp
    JSONProductStorage.cpp
    JSONReceiptStorage.cpp
    JSONInventoryStorage.cpp
    JSONCustomerStorage.cpp
)

target_include_directories(Infrastructure PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Infrastructure PUBLIC Core Bussiness)