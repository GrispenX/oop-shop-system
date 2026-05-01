add_library(Bussiness
    RegularDiscount.cpp
    BundleDiscount.cpp
    ProductService.cpp
    DiscountService.cpp
    ReceiptService.cpp
)

target_include_directories(Bussiness PUBLIC ${CMAKE_SOURCE_DIR}/src)