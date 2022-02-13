#pragma once

#include <memory>
#include <asio.hpp>

namespace sserver {

class server_pimpl;

class server
{
public:
    server(asio::ip::tcp::endpoint ep);
    ~server();
    bool isRun();
private:
    std::unique_ptr<server_pimpl> m_pimpl;
};

} // namespace chat
