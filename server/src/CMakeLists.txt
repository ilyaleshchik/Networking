cmake_minimum_required(VERSION 3.10)

project(server VERSION 2.28)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(ServerSource)

add_executable(server main.cpp)
target_link_libraries(server PUBLIC serverlib)
