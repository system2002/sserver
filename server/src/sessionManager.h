#pragma once
#include <unordered_set>
#include <memory>
#include <asio.hpp>
#include "session.h"

namespace sserver {
using pSession = std::shared_ptr<session>;

class sessionManager
{
public:
    sessionManager(asio::io_context &context) :
        m_context {context}
    {}

    bool contains(pSession session) const;
    void insert(pSession session) { m_sessions.insert(session); }
    void erase(pSession session);
    size_t count() const {return m_sessions.size();}
    pSession newSession();

private:
    std::unordered_set<pSession> m_sessions;
    asio::io_context & m_context;
};
}
