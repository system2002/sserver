add_library(serverLib
    include/server/server.h

    src/server_impl.h
    src/sessionManager.h
    src/session.h
    src/server_impl.cpp
    src/sessionManager.cpp
    src/session.cpp
    src/server.cpp
    )

target_link_libraries(serverLib ASIO::ASIO)
target_include_directories(serverLib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)

add_subdirectory(sandbox)




