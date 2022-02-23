#include "handlemanager.h"
#include <sstream>

namespace sserver {
handleManager & handleManager::getManager()
{
    static handleManager thisManager {};
    return thisManager;
}

handleManager::handleManager()
{

}

void handleManager::callHandle(const request & req , repler & reply) const
{
    handle_type handle = [](const request & req, repler & reply)
    {
        reply.addHeaderLine("Content-Type", "text/html; charset=utf-8");
        if (req.uri == "/")
        {
            std::stringstream html {};
            html            << "<title> Small C++ http server</title>\n"
                            << "<h1>Server good</h1>\n"
                            << "<h2>Request headers</h2>\n";
//            html << "Client:" << m_socket.remote_endpoint().address().to_string() << "</br>\n"  ;
//            html << "Client id:" << uint64_t(shared_from_this().get()) << "</br>\n"  ;
//            html << "Connection count:" << m_manager.count() << "</br>\n"  ;
            html << "URI:" << req.uri << "</br>\n"  ;
            html << "<ul>\n";
            for (auto && line : req.headers)
            {
                html <<"<li>"<< line.name << ": " << line.value << "</li>\n"  ;
            }
            html << "</ul>\n";
            html << "<h2>Small C++ http server</h2>\n";
            reply.setContent(html.str());
        }
    };
    handle(req, reply);
}

void handleManager::insertEqual(std::string_view uriKey, handle_type &&handle )
{
    if (!handle) return;
    m_equalHandle.insert(std::make_pair(uriKey, handle));

}
}
