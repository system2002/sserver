#include "sessionManager.h"
#include "session.h"
namespace sserver {

bool sessionManager::contains(pSession session) const
{
    return m_sessions.find(session) != m_sessions.cend();
}

void sessionManager::erase(pSession session)
{
    if (contains(session))
    {
        m_sessions.erase(session);
        std::cout << "session destroy" << std::endl;
    }
}

pSession sessionManager::makeSession()
{
    return std::make_shared<session>(m_context, *this);
}

}
