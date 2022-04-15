#pragma once

#include <memory>
#include <asio.hpp>
#include <functional>
#include "server/repler.h"
#include "server/request.hpp"

namespace sserver {

class repler;
class server_impl;
using handle_type = std::function<void(const request&, repler &)>;


class server
{
public:
    server(asio::ip::tcp::endpoint ep);
    ~server();
    void appendEqualHandle(std::string_view equalUri, handle_type &&handle);
    void appendPrefixHandle(std::string_view equalUri, handle_type &&handle);

    bool isRun() const;
private:
    std::unique_ptr<server_impl> pimpl_;
};
}
