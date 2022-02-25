#pragma once
#include "string"
#include "types.h"
#include "asio/buffer.hpp"

namespace sserver {
enum class replyStatus_t
{
    ok = 200,
    created = 201,
    accepted = 202,
    no_content = 204,
    multiple_choices = 300,
    moved_permanently = 301,
    moved_temporarily = 302,
    not_modified = 304,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503
};
enum class connection_t
{
    Close,
    keep_alive
};

class repler
{
public:
    repler(replyStatus_t status = replyStatus_t::ok);
    void setStatus(replyStatus_t status) {m_replyStatus = status;}
    void setContentLength(size_t length);
    void setConnectionType(connection_t connection);

    template<typename NameString, typename valueString>
    void addHeaderLine(NameString && name, valueString && value)
    {
        m_headerLines.emplace_back(std::forward<NameString>(name), std::forward<valueString>(value));
    }

    template<typename ContentString>
    void setContent(ContentString &&content, bool setLineContentLength = true)
    {
        m_content = std::forward<ContentString>(content);
        if (setLineContentLength)
        {
            setContentLength(m_content.length());
        }
    }
    std::vector<asio::const_buffer> toBuffer();
private:
    static std::string_view status_text(replyStatus_t reply) ;
    headers_t m_headerLines;
    replyStatus_t m_replyStatus;
    std::string m_content;
};


}
