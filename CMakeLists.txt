cmake_minimum_required(VERSION 3.16)

project(thread_pool)

include_directories(./include)

find_package(Threads REQUIRED)

add_executable(thread_pool main.cpp)

target_link_libraries(thread_pool ${Threads_LIBRARIES})

set_target_properties(thread_pool PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(thread_pool 
PRIVATE Threads::Threads
)

