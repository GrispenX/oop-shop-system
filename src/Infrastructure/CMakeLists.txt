add_library(Infrastructure
    ProductService.cpp
    ReceiptService.cpp
)

target_include_directories(Infrastructure PUBLIC ${CMAKE_SOURCE_DIR}/src)