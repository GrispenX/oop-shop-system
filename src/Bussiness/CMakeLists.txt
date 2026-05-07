add_library(Bussiness
    RegularDiscount.cpp
    BundleDiscount.cpp
)

target_include_directories(Bussiness PUBLIC ${CMAKE_SOURCE_DIR}/src)