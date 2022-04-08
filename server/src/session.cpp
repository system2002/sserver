#include "session.h"
#include "sessionManager.h"
#include "server/repler.h"

namespace sserver {

session::session(asio::io_context &context, sessionManager &managerRef):
    m_socket{context},
    m_readBuffer (4096),
    m_manager {managerRef},
    m_handler {handleManager::getManager()}
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
        m_handler.callHandle(requestCurr, reply);
    }
}

void session::closeSession()
{
    auto self = shared_from_this();
    m_manager.erase(shared_from_this());
}
}
