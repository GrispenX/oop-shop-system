add_subdirectory(Bussiness)
add_subdirectory(Console)
add_subdirectory(Core)
add_subdirectory(Infrastructure)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    Core
    Bussiness
    Console
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)