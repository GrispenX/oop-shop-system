add_library(Bussiness
    RegularDiscount.cpp
    BundleDiscount.cpp
    RegularCashback.cpp
)

target_include_directories(Bussiness PUBLIC ${CMAKE_SOURCE_DIR}/src)