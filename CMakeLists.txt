cmake_minimum_required(VERSION 3.12)

project(sserver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

list(APPEND CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(ASIO_ROOT ${CMAKE_SOURCE_DIR}/external/asio/asio)

find_package(ASIO REQUIRED)
add_subdirectory(server)
add_subdirectory(client_tester)
