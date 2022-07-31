add_library(serverlib server.cpp)
target_include_directories(serverlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
