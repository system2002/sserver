#include "session.h"
#include "sessionManager.h"

namespace sserver {

session::session(asio::io_context &context, sessionManager &managerRef):
    m_socket{context},
    m_readBuffer (4096),
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
    auto self(shared_from_this());
    m_socket.async_read_some(asio::buffer(m_readBuffer),
        [this, self](std::error_code ec, std::size_t bytes_transferred)
        {
            if (!ec)
            {
                request_parser::result_t result;
                std::tie(result, std::ignore) = m_parcer.parse(
                m_request, m_readBuffer.data(), m_readBuffer.data() + bytes_transferred);
                if (result != request_parser::result_t::indeterminate)
                {
                    makeReply(result);
                    asyncSendData(asio::buffer(m_reply.str()));
                    m_reply.clear(std::_Ios_Iostate::_S_eofbit);
                    m_parcer.reset();
                    m_request.reset();
                    asyncRead();
                }
                else
                {
                    asyncRead();
                }

            } else {
                closeSession();
            }
        });
}

void session::asyncWrite()
{
    auto self(shared_from_this());
    asio::async_write(m_socket, asio::buffer(asio::buffer(m_reply.str())),
        [this, self](std::error_code ec, std::size_t)
        {
          if (ec)
          {
              std::cerr << "Close client" << std::endl;
              closeSession();
          }
    });
}

void session::makeReply(request_parser::result_t res)
{
    if (res == request_parser::result_t::good)
    {
        std::cout << "uri: " <<  m_request.uri << "\n"  ;
        std::stringstream html {};
        html            << "<title> Small C++ http server</title>\n"
                        << "<h1>Server good</h1>\n"
                        << "<h2>Request headers</h2>\n";
        html <<"<p>client:"<< m_socket.remote_endpoint().address().to_string() << "</p>\n"  ;
        html <<"<p>URI:"<< m_request.uri << "</p>\n"  ;

                      for (auto line : m_request.headers)
                      {
                          html <<"<p>"<< line.name << " : " << line.value << "</p>\n"  ;
                      }


        html            << "<small>Small C++ http server</small>\n";

        m_reply         << "HTTP/1.1 200 OK\r\n"
                        << "Version: HTTP/1.1\r\n"
                        << "Connection : keep-alive\r\n"
                        << "Cache-Control : max-age=0\r\n"
                        << "Content-Type: text/html; charset=utf-8\r\n"
                        << "Content-Length: " << html.str().length()
                        << "\r\n\r\n"
                        << html.str();
    }
    else
    {
        std::stringstream html {};
        html            << "<title> Small C++ http server</title>\n"
                        << "<h1>bad</h1>\n";

        m_reply         << "HTTP/1.1 200 OK\r\n"
                        << "Version: HTTP/1.1\r\n"
                        << "Content-Type: text/html; charset=utf-8\r\n"
                        << "Content-Length: " << html.str().length()
                        << "\r\n\r\n"
                        << html.str();
    }
}

void session::closeSession()
{
    auto self = shared_from_this();
    m_manager.erase(shared_from_this());

}
}
