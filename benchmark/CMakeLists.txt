cmake_minimum_required(VERSION 3.16)
project(benchmark)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(${PROJECT_NAME} 
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/benchmark.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/cv_queue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/mutex_queue.h
)

find_package(CLI11 REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE lockfree_spsc_queue)
target_link_libraries(${PROJECT_NAME} PRIVATE CLI11::CLI11)

