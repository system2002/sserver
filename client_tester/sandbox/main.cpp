#include <iostream>
#include <asio.hpp>
#include <thread>
#include <atomic>
#include <chrono>
#include "client_tester/clientmanager.h"

int main()
{
//    std::atomic_bool stop = false;

    asio::io_context context;
//    asio::ip::tcp::endpoint ep{asio::ip::address_v4::loopback(), 10000};
//    std::cout << ep.address().to_string() << ":" << ep.port() << std::endl;
    asio::signal_set signal(context, SIGINT, SIGTERM);
    signal.async_wait([&context](const asio::error_code & err, int signal)
        {
            context.stop();
        });
    cl::clientManager cl{context};

//    std::string request =
//            "GET / HTTP/1.1\r\n"
//            "User-agent: client\r\n"
//            "Host: localhost:8000\r\n\r\n";

    cl.sendRequest();
//    auto res = std::async(std::launch::async, [&context]()
//    {
//        context.run();
////        stop = true;
//    });
    constexpr size_t thead_count = 4;
    std::vector<std::future<void>> vf (thead_count);
    for (auto && future : vf)
    {
        future = std::async(std::launch::async, [&]()
        {
            context.run();
        });
    }

    for (auto && future : vf)
    {
        future.wait();
    }

    std::cout << "Exit" << std::endl;
    return 0;
}
