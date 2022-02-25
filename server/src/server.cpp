#include "server/server.h"
#include "server_impl.h"
namespace sserver {

server::server(asio::ip::tcp::endpoint ep)
{
    std::unique_ptr<server_pimpl> server;
    try
    {
        server = std::make_unique<server_pimpl>(ep);
    }
    catch (std::system_error error)
    {
        std::cerr << "Error init server " << error.what() << " error code: " << error.code() << std::endl;
    }
    if (server != nullptr)
    {
        m_pimpl = std::move(server);
    }
}

void server::appendEqualHandle(std::string_view equalUri, handle_type &&handle)
{
    if (m_pimpl)
    {
        m_pimpl->appendEqualHandle(equalUri, std::move(handle));
    }
}

void server::appendPrefixHandle(std::string_view equalUri, handle_type &&handle)
{
    if (m_pimpl)
    {
        m_pimpl->appendPrefixHandle(equalUri, std::move(handle));
    }
}

bool server::isRun()
{
    return m_pimpl && m_pimpl->isRun();
}
server::~server() = default;

}
