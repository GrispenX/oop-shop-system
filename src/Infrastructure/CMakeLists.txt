add_library(Infrastructure
    ProductService.cpp
    ReceiptService.cpp
    JSONProductStorage.cpp
    JSONReceiptStorage.cpp
)

target_include_directories(Infrastructure PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Infrastructure PUBLIC Core Bussiness)