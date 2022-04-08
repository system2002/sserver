#pragma once
#include <atomic>
#include "client_tester/client.h"


constexpr size_t connect_count = 200;

namespace cl
{
class clientManager
{
public:
    clientManager(asio::io_context & context);
    void sendRequest();

private:
    std::atomic<size_t> count_;
    asio::io_context & context_;
    std::vector<client> vclient_;
    CallbackRead_t handle_;

};

}

