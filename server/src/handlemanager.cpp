#include "handlemanager.h"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace sserver {
handleManager & handleManager::getManager()
{
    static handleManager thisManager {};
    return thisManager;
}

handleManager::handleManager()
{
    handle_type handle = [](const request & req, repler & reply)
    {
        reply.addHeaderLine("Content-Type", "text/html; charset=utf-8");
        std::stringstream html {};
        html            << "<title> Small C++ http server</title>\n"
                           << "<h1>Server good</h1>\n"
                           << "<h2>Request headers</h2>\n";
        html << "URI:" << req.uri << "</br>\n"  ;
        html << "<ul>\n";
        for (auto && line : req.headers)
        {
            html <<"<li>"<< line.name << ": " << line.value << "</li>\n"  ;
        }
        html << "</ul>\n";
        html << "<h2>Small C++ http server</h2>\n";
        reply.setContent(html.str());

    };
    appendPrefixHandle("/", std::move(handle));
}

void handleManager::callHandle(const request & req , repler & reply) const
{
    auto it = m_equalHandle.find(req.uri);
    if (it != m_equalHandle.cend())
    {
        it->second(req, reply);
        return;
    }

    auto find = m_prefixHandle.cend();
    for (auto it = m_prefixHandle.cbegin(); it != m_prefixHandle.cend(); ++it)
    {
        if (req.uri.size() < it->first.size()) continue;
        std::string_view uriPrefix = std::string_view(req.uri).substr(0, it->first.size());
        if (uriPrefix == it->first)
        {
            if (find == m_prefixHandle.cend())
            {
                find = it;
            }
            else
            {
                if (find->first.size() < it->first.size())
                {
                    find = it;
                }
            }
        }
    }
    if (find != m_prefixHandle.cend())
    {
        find->second(req, reply);
    }
}

void handleManager::appendEqualHandle(std::string_view uriKey, handle_type &&handle )
{
    if (!handle) return;
    m_equalHandle[static_cast<std::string>(uriKey)] = handle;
}

void handleManager::appendPrefixHandle(std::string_view uriKey, handleManager::handle_type &&handle)
{
    if (!handle) return;
    m_prefixHandle[static_cast<std::string>(uriKey)] = handle;
}
}
