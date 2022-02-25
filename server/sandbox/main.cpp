#include <iostream>
#include <future>
#include "server/server.h"

void signal_handler(asio::io_context &context, const asio::error_code &, int)
{
    std::cout << "Server stop" << std::endl;
    context.stop();
}

int main()
{
    asio::io_context context;
    asio::ip::tcp::endpoint ep {asio::ip::address_v4::any(), 8000};
    sserver::server srv {ep};
    if (!srv.isRun()) return 1;

    // Start page
    srv.appendEqualHandle("/", [](const sserver::request& req, sserver::repler &reply)
    {
        reply.addHeaderLine("Content-Type", "text/html; charset=utf-8");
        std::stringstream html {};
        html
            << "<title> Small C++ http server. About</title>\n"
            << "<h1>work it</h1>\n"
            << "<h6>Small C++ http server</h6>\n";
        reply.setContent(html.str());
    });

    // about page
    srv.appendEqualHandle("/about", [](const sserver::request& req, sserver::repler &reply)
    {
        reply.addHeaderLine("Content-Type", "text/html; charset=utf-8");
        std::stringstream html {};
        html
            << "<title> Small C++ http server. About</title>\n"
            << "<h1>About</h1>\n"
            << "<h6>Small C++ http server</h6>\n"
            << "contain host" << (req.headerContains("host") ? "+" : "-");
        reply.setContent(html.str());
    });

    // default page

//    srv.appendPrefixHandle("/", [](const sserver::request& req, sserver::repler &reply)
//    {
//        reply.addHeaderLine("Content-Type", "text/html; charset=utf-8");
//        reply.setStatus(sserver::replyStatus_t::forbidden);
//        std::stringstream html {};
//        html
//            << "<title> Small C++ http server. About</title>\n"
//            << "<h1>Forbidden</h1>\n"
//            << "<h6>Small C++ http server</h6>\n";
//        reply.setContent(html.str());
//    });

    srv.appendPrefixHandle("/news", [](const sserver::request& req, sserver::repler &reply)
    {
        reply.addHeaderLine("Content-Type", "text/html; charset=utf-8");
        std::stringstream html {};
        html
            << "<title> Small C++ http server. News</title>\n"
            << "<h1>News</h1>\n"
            << "URI: " << req.uri
            << "<h6>Small C++ http server</h6>\n";
        reply.setContent(html.str());
    });
    srv.appendPrefixHandle("/new", [](const sserver::request& req, sserver::repler &reply)
    {
        reply.addHeaderLine("Content-Type", "text/html; charset=utf-8");
        std::stringstream html {};
        html
            << "<title> Small C++ http server. New</title>\n"
            << "<h1>New</h1>\n"
            << "URI: " << req.uri
            << "<h2>Small C++ http server</h2>\n";
        reply.setContent(html.str());
    });

    constexpr size_t thead_count = 12;
    std::vector<std::future<void>> vf (thead_count);

    asio::signal_set signal(context, SIGINT, SIGTERM);
    signal.async_wait([&context](const asio::error_code & err, int signal)
    {
        signal_handler(context, err, signal);
    });


//    std::future<void> future = std::async(std::launch::async, [&]()
//    {
//        context.run();
//    });

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
    return 0;
}
