#include "client_tester/clientmanager.h"
#include <algorithm>
namespace cl
{

clientManager::clientManager(asio::io_context &context) :
    count_ {0},
    context_ {context}

{
    handle_ = [this]() -> void
    {
            count_++;
            std::cout << count_<< "             \r";
    };
    vclient_.reserve(connect_count);
    for (size_t i = 0; i < connect_count; ++i)
    {
        vclient_.emplace_back(context_, handle_);
    }
    std::for_each(vclient_.begin(), vclient_.end(), [](client & cl)
    {
        cl.connect();
    });
}

void clientManager::sendRequest()
{
    std::for_each(vclient_.begin(), vclient_.end(), [](client & cl)
    {
        cl.staticSend();
    });
}

}
