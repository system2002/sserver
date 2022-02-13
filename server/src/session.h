#pragma once
#include <iostream>
#include <asio.hpp>

namespace sserver {
class sessionManager;

class session: public std::enable_shared_from_this<session>
{
public:
    session (asio::io_context & context, sessionManager & managerRef);
    ~session ();
    void start();
    void asyncRead();
    void readHandler(const asio::error_code& error, std::size_t size);
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
    asio::ip::tcp::socket m_socket;
    std::vector<char> m_readBuffer;
    sessionManager & m_manager;
    void closeSession();
};
}
