add_library(serverLib
    include/server/server.h
    src/server_impl.h
    src/sessionManager.h
    src/session.h
    src/server_impl.cpp
    src/sessionManager.cpp
    src/session.cpp
    src/server.cpp
    include/server/request.hpp
    src/request.cpp
    src/request_parser.hpp
    src/request_parser.cpp
    include/server/repler.h
    src/repler.cpp
    include/server/types.h
    src/handlemanager.cpp
    src/handlemanager.h
    )

target_link_libraries(serverLib ASIO::ASIO)
target_include_directories(serverLib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)

add_subdirectory(sandbox)




