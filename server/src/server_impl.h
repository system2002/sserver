#pragma once

#include <iostream>
#include <asio.hpp>
#include <future>
#include "sessionManager.h"
#include "handlemanager.h"


namespace sserver {

class server_pimpl
{
public:
    server_pimpl(asio::ip::tcp::endpoint ep);
    server_pimpl(const server_pimpl &) = delete;
    server_pimpl& operator=(const server_pimpl&) = delete;
    ~server_pimpl();
    void appendEqualHandle(std::string_view equalUri, handleManager::handle_type &&);


    bool isRun();
private:
    asio::io_context m_context;
    asio::ip::tcp::acceptor m_acceptor;
    void handleAccept(const asio::error_code& error, pSession newSession);
    void startAccept();
    sessionManager m_manager;
    std::future<void> m_asyncContextRun;
};
} //namespace chat
