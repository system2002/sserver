#include "server/server.h"
#include "server_impl.h"
namespace sserver {

server::server(asio::ip::tcp::endpoint ep)
{
    try
    {
        std::unique_ptr<server_pimpl> server = std::make_unique<server_pimpl>(ep);
        if (server != nullptr)
        {
            m_pimpl = std::move(server);
        }
    }  catch (std::system_error error)
    {
        std::cerr << "Error init server " << error.what() << " error code: " << error.code() << std::endl;
    }

}

bool server::isRun()
{
    return m_pimpl && m_pimpl->isRun();
}
server::~server() = default;

}
