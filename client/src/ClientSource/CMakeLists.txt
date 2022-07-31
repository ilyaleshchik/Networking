add_library(clientlib client.cpp)
target_include_directories(clientlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})