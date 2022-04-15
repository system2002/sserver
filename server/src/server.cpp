#include "server/server.h"
#include "server_impl.h"
namespace sserver {

server::server(asio::ip::tcp::endpoint ep)
{
    std::unique_ptr<server_impl> server;
    try
    {
        server = std::make_unique<server_impl>(ep);
    }
    catch (std::system_error error)
    {
        std::cerr << "Error init server " << error.what() << " error code: " << error.code() << std::endl;
        return;
    }
    if (server != nullptr)
    {
        pimpl_ = std::move(server);
    }
}

void server::appendEqualHandle(std::string_view equalUri, handle_type &&handle)
{
    if (pimpl_)
    {
        pimpl_->appendEqualHandle(equalUri, std::move(handle));
    }
}

void server::appendPrefixHandle(std::string_view equalUri, handle_type &&handle)
{
    if (pimpl_)
    {
        pimpl_->appendPrefixHandle(equalUri, std::move(handle));
    }
}

bool server::isRun() const
{
    return pimpl_ && pimpl_->isRun();
}
server::~server() = default;

}
