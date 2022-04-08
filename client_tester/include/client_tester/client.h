#pragma once
#include <iostream>
#include <vector>
#include "asio.hpp"
namespace cl
{

    using CallbackRead_t = std::function<void(void)>;

class client
{
public:
    client(asio::io_context &, CallbackRead_t &);
    void connect();
    void asyncSendData(const std::string &text)
    {
        asyncSendData(asio::buffer(text));
    }

    template<typename Buffer_t>
    void asyncSendData(const Buffer_t &buffer)
    {
        socket_.async_write_some(buffer, [&](const asio::error_code& error, std::size_t size)
        {
            if (error)
            {
                std::cerr << "Send error: " << error.value();
            }
        });
    }
    void staticSend();




private:
    asio::io_context &context_;
    asio::ip::tcp::socket socket_;
    void asyncRead();
    std::vector<char> readBuffer_;
    CallbackRead_t handle_;
};

}
