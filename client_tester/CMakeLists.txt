add_library(clientLib STATIC
    include/client_tester/client.h
    src/client.cpp
    include/client_tester/clientmanager.h
    src/clientmanager.cpp
    )

target_link_libraries(clientLib ASIO::ASIO)
target_include_directories(clientLib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)

add_subdirectory(sandbox)




