#include "session.h"
#include "sessionManager.h"

namespace sserver {

session::session(asio::io_context &context, sessionManager &managerRef):
    m_socket{context},
    m_readBuffer (1024),
    m_manager {managerRef}
{}

session::~session()
{

}

void session::start()
{
    asyncRead();
}

void session::asyncRead()
{
    m_socket.async_read_some(asio::buffer(m_readBuffer), [&](const asio::error_code& error, std::size_t size)
    {
        if (!error)
        {
            readHandler(error, size);
            asyncRead();
        }
        else
        {
            std::cerr << "Close client" << std::endl;
            closeSession();
        }
    });
}

void session::readHandler(const asio::error_code &error, std::size_t size)
{
//    std::cout << "read: ";
//    std::cout.write(m_readBuffer.data(), size) << std::endl;
    std::string request(m_readBuffer.begin(), m_readBuffer.begin() + size);
    std::stringstream response;
    std::stringstream html;
    html            << "<title> Small C++ http server</title>\n"
                    << "<h1>Server</h1>\n"
                    << "<h2>Request headers</h2>\n"
                    << "<pre>" << request << "</pre>\n"
                    << "<small>Small C++ http server</small>\n";


    response        << "HTTP/1.1 200 OK\r\n"
                    << "Version: HTTP/1.1\r\n"
                    << "Content-Type: text/html; charset=utf-8\r\n"
                    << "Content-Length: " << html.str().length()
                    << "\r\n\r\n"
                    << html.str();





    asyncSendData(asio::buffer(response.str()));
}

void session::closeSession()
{
    auto self = shared_from_this();
    m_manager.erase(shared_from_this());

}
}
