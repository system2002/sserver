#include "server_impl.h"
#include "session.h"

namespace sserver {

server_pimpl::server_pimpl(asio::ip::tcp::endpoint ep):
    m_acceptor {m_context, ep},
    m_manager(m_context)
{
    startAccept();
    m_asyncContextRun = std::async(std::launch::async, [this]
    {
        m_context.run();
    });
}

server_pimpl::~server_pimpl()
{
    m_context.stop();
}

bool server_pimpl::isRun()
{
    return !m_context.stopped();
}

void server_pimpl::handleAccept(const asio::error_code &error, pSession newSession)
{
    if (!error)
    {
        m_manager.insert(newSession);
        std::cout << "Accepted! client count: " << m_manager.count() << std::endl;
        newSession->start();
        startAccept();
    }
    else
    {
        std::cout << "Accept error: " << error.value() << std::endl;
    }
}

void server_pimpl::startAccept()
{
    pSession newSession = m_manager.newSession();
    m_acceptor.async_accept(newSession->socket(), [this, newSession](const asio::error_code &error)
    {
        if (!error)
        {
            handleAccept(error, newSession);
        }
        else
        {
            std::cout << "Accept error" << std::endl;
        }
    });
}

}
