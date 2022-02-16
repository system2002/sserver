#pragma once
#include <iostream>
#include <asio.hpp>
#include "request.hpp"
#include "request_parser.hpp"

namespace sserver {
class sessionManager;

class session: public std::enable_shared_from_this<session>
{
public:
    session (asio::io_context & context, sessionManager & managerRef);
    ~session ();
    void start();
    void asyncRead();
    asio::ip::tcp::socket &socket() { return m_socket; }

    template<typename Buffer_t>
    void asyncSendData(Buffer_t &&buffer)
    {
        m_socket.async_write_some(std::forward<Buffer_t>(buffer), [&](const asio::error_code& error, std::size_t)
        {
            if (error)
            {
                std::cerr << "send error: " << error.value() << std::endl;
            }
        });
    }

private:
    void asyncWrite();
    void makeReply(request_parser::result_t &&);
    asio::ip::tcp::socket m_socket;
    std::vector<char> m_readBuffer;
    sessionManager & m_manager;
    request m_request;
    request_parser m_parcer;
    std::stringstream m_reply;
    void closeSession();
};
}
