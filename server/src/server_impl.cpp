#include "server_impl.h"
#include "session.h"

namespace sserver {

server_impl::server_impl(asio::ip::tcp::endpoint ep):
    m_acceptor {m_context, ep},
    m_manager(m_context)
{
    startAccept();
    m_asyncContextRun = std::async(std::launch::async, [this]
    {
        m_context.run();
    });
}

server_impl::~server_impl()
{
    m_context.stop();
}

void server_impl::appendEqualHandle(std::string_view equalUri, handleManager::handle_type && handle)
{
    handleManager::getManager().appendEqualHandle(equalUri, std::move(handle));
}

void server_impl::appendPrefixHandle(std::string_view equalUri, handleManager::handle_type && handle)
{
    handleManager::getManager().appendPrefixHandle(equalUri, std::move(handle));
}

bool server_impl::isRun() const
{
    return !m_context.stopped();
}

void server_impl::handleAccept(const asio::error_code &error, pSession newSession)
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

void server_impl::startAccept()
{
    pSession newSession = m_manager.makeSession();
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
