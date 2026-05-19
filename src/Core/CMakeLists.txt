add_library(Core
    Product.cpp
    ReceiptItem.cpp
    Receipt.cpp
    Customer.cpp
)

target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/src)