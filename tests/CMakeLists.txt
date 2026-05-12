find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_test
    Helpers.cpp
    ReceiptTest.cpp
    DiscountTest.cpp
    ProductServiceTest.cpp
    ReceiptServiceTest.cpp
    CashbackServiceTest.cpp
)

target_link_libraries(${PROJECT_NAME}_test PRIVATE
    Core
    Bussiness
    Infrastructure
)

target_include_directories(${PROJECT_NAME}_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)