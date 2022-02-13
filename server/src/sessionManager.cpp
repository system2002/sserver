#include "sessionManager.h"
#include "session.h"
namespace sserver {

bool sessionManager::contains(pSession session) const
{
    for (auto it = m_sessions.cbegin(); it != m_sessions.cend(); ++it)
    {
        if (session == *it) return true;
    }
    return false;
}

void sessionManager::erase(pSession session)
{
    if (contains(session))
    {
        m_sessions.erase(session);
        std::cout << "session destroy" << std::endl;
    }
}

pSession sessionManager::newSession()
{
    return std::make_shared<session>(m_context, *this);
}

}
