#include "client_tester/client.h"


constexpr std::string_view request =
        "GET / HTTP/1.1\r\n"
        "User-agent: client\r\n"
        "Host: localhost:8000\r\n\r\n";

const asio::ip::tcp::endpoint ep {asio::ip::address_v4::from_string("127.0.0.1"), 8000};
namespace cl
{

client::client(asio::io_context & context, CallbackRead_t & handle) :
    context_ {context},
    socket_ {context},
    readBuffer_ (1024),
    handle_ {handle}
{}

void client::connect()
{
        socket_.async_connect(ep, [this](const asio::error_code &ec)
        {
            if (!ec)
            {
                std::cout << "Connect Ok" << std::endl;
                asyncRead();
            }
            else
            {
                std::cerr << "Connect error: " << ec.message() << std::endl;
            }
        });
}

void client::staticSend()
{
    asyncSendData(std::string(request));
}

void client::asyncRead()
{
    socket_.async_read_some(asio::buffer(readBuffer_), [&](const asio::error_code& error, std::size_t size)
    {
        if (!error)
        {
            handle_();
            asyncRead();
            staticSend();
        }
        else
        {
            std::cerr << "Server close\n";
        }
    });
}

}
