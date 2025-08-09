project(lockfree_tests)

find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spsc_tests.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE lockfree_spsc_queue)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})