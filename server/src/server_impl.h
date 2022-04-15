#pragma once

#include <iostream>
#include <asio.hpp>
#include <future>
#include "sessionManager.h"
#include "handlemanager.h"


namespace sserver {

class server_impl
{
public:
    server_impl(asio::ip::tcp::endpoint ep);
    server_impl(const server_impl &) = delete;
    server_impl& operator=(const server_impl&) = delete;
    ~server_impl();
    void appendEqualHandle(std::string_view equalUri, handleManager::handle_type &&);
    void appendPrefixHandle(std::string_view equalUri, handleManager::handle_type &&);


    bool isRun() const;
private:
    asio::io_context m_context;
    asio::ip::tcp::acceptor m_acceptor;
    void handleAccept(const asio::error_code& error, pSession newSession);
    void startAccept();
    sessionManager m_manager;
    std::future<void> m_asyncContextRun;
};
} //namespace chat
