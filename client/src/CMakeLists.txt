cmake_minimum_required(VERSION 3.10)

project(client VERSION 2.28)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_subdirectory(ClientSource)

add_executable(client main.cpp)

target_link_libraries(client PUBLIC clientlib)