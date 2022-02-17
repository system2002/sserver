#include "session.h"
#include "sessionManager.h"
#include "repler.h"

namespace sserver {

session::session(asio::io_context &context, sessionManager &managerRef):
    m_socket{context},
    m_readBuffer (4096),
    m_manager {managerRef}
{}

session::~session()
{}

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
            request_parser::result_t result = m_parcer.parse(
                        m_request, m_readBuffer.data(), m_readBuffer.data() + bytes_transferred);
            if (result != request_parser::result_t::indeterminate)
            {
                repler reply;
                makeReply(result, m_request, reply);
                asyncSendData(reply.toBuffer());
                m_reply.clear(std::_Ios_Iostate::_S_eofbit);
                m_parcer.reset();
                m_request.reset();
                asyncRead();
            }
            else
            {
                asyncRead();
            }

        }
        else
        {
            closeSession();
        }
    });
}

void session::makeReply(const request_parser::result_t &res, const request & requestCurr, repler & reply)
{
    if (res == request_parser::result_t::good)
    {
        reply.addHeaderLine("Content-Type", "text/html; charset=utf-8");
        if (requestCurr.uri == "/")
        {
            std::stringstream html {};
            html            << "<title> Small C++ http server</title>\n"
                            << "<h1>Server good</h1>\n"
                            << "<h2>Request headers</h2>\n";
            html << "Client:" << m_socket.remote_endpoint().address().to_string() << "</br>\n"  ;
            html << "Client id:" << uint64_t(shared_from_this().get()) << "</br>\n"  ;
            html << "Connection count:" << m_manager.count() << "</br>\n"  ;
            html << "URI:" << m_request.uri << "</br>\n"  ;
            html << "<ul>\n";
            for (auto &line : m_request.headers)
            {
                html <<"<li>"<< line.name << ": " << line.value << "</li>\n"  ;
            }
            html << "</ul>\n";
            html << "<h2>Small C++ http server</h2>\n";
            reply.setContent(html.str());
        }
        else
        {
            reply.setStatus(replyStatus_t::forbidden);
            std::stringstream html {};
            html << "<title> Small C++ http server</title>\n"
                 << "<h1>forbidden</h1>\n";
            if (requestCurr.headerContains("Host"))
            {
                html << "forbidden url: http://"
                     << (requestCurr.getValueHeaders("Host")) << requestCurr.uri << "\n";
            }
            reply.setContent(html.str());
        }
    }
}

void session::closeSession()
{
    auto self = shared_from_this();
    m_manager.erase(shared_from_this());
}
}
