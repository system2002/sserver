#include <string>
#include <sstream>
#include <algorithm>
#include "server/repler.h"

namespace  http_protocol_str{
constexpr std::string_view ok =
        "HTTP/1.0 200 OK\r\n";
constexpr std::string_view created =
        "HTTP/1.0 201 Created\r\n";
constexpr std::string_view accepted =
        "HTTP/1.0 202 Accepted\r\n";
constexpr std::string_view no_content =
        "HTTP/1.0 204 No Content\r\n";
constexpr std::string_view multiple_choices =
        "HTTP/1.0 300 Multiple Choices\r\n";
constexpr std::string_view moved_permanently =
        "HTTP/1.0 301 Moved Permanently\r\n";
constexpr std::string_view moved_temporarily =
        "HTTP/1.0 302 Moved Temporarily\r\n";
constexpr std::string_view not_modified =
        "HTTP/1.0 304 Not Modified\r\n";
constexpr std::string_view bad_request =
        "HTTP/1.0 400 Bad Request\r\n";
constexpr std::string_view unauthorized =
        "HTTP/1.0 401 Unauthorized\r\n";
constexpr std::string_view forbidden =
        "HTTP/1.0 403 Forbidden\r\n";
constexpr std::string_view not_found =
        "HTTP/1.0 404 Not Found\r\n";
constexpr std::string_view internal_server_error =
        "HTTP/1.0 500 Internal Server Error\r\n";
constexpr std::string_view not_implemented =
        "HTTP/1.0 501 Not Implemented\r\n";
constexpr std::string_view bad_gateway =
        "HTTP/1.0 502 Bad Gateway\r\n";
constexpr std::string_view service_unavailable =
        "HTTP/1.0 503 Service Unavailable\r\n";

constexpr std::string_view name_value_glue = ": ";
constexpr std::string_view end_line = "\r\n";
constexpr std::string_view content_length = "Content-Length";
constexpr std::string_view connection = "Connection";
constexpr std::string_view keep_alive = "keep-alive";
constexpr std::string_view close = "close";

}

namespace sserver {

repler::repler(replyStatus_t status)  : replyStatus_ {status}
{
    setConnectionType(connection_t::keep_alive);
}

std::string_view repler::status_text(replyStatus_t reply)
{
    switch (reply)
    {
    case replyStatus_t::ok:                     return http_protocol_str::ok;
    case replyStatus_t::created:                return http_protocol_str::created;
    case replyStatus_t::accepted:               return http_protocol_str::accepted;
    case replyStatus_t::no_content:             return http_protocol_str::no_content;
    case replyStatus_t::multiple_choices:       return http_protocol_str::multiple_choices;
    case replyStatus_t::moved_permanently:      return http_protocol_str::moved_permanently;
    case replyStatus_t::moved_temporarily:      return http_protocol_str::moved_temporarily;
    case replyStatus_t::not_modified:           return http_protocol_str::not_modified;
    case replyStatus_t::bad_request:            return http_protocol_str::bad_request;
    case replyStatus_t::unauthorized:           return http_protocol_str::unauthorized;
    case replyStatus_t::forbidden:              return http_protocol_str::forbidden;
    case replyStatus_t::not_found:              return http_protocol_str::not_found;
    case replyStatus_t::internal_server_error:  return http_protocol_str::internal_server_error;
    case replyStatus_t::not_implemented:        return http_protocol_str::not_implemented;
    case replyStatus_t::bad_gateway:            return http_protocol_str::bad_gateway;
    case replyStatus_t::service_unavailable:    return http_protocol_str::service_unavailable;
    default:                                    return http_protocol_str::internal_server_error;
    }
}

void repler::setContentLength(size_t length)
{
    auto it = std::find_if(headerLines_.begin(), headerLines_.end(), [](headerLine &line)
    {
        return line.name == http_protocol_str::content_length;
    });

    if (it != headerLines_.end())
    {
        it->value = std::to_string(length);
    }
    else
    {
        headerLines_.emplace_back(http_protocol_str::content_length, std::to_string(length));
    }
}

void repler::setConnectionType(connection_t conn)
{
    addHeaderLine(http_protocol_str::connection,
                  (conn == connection_t::keep_alive) ? http_protocol_str::keep_alive : http_protocol_str::close);
}

std::vector<asio::const_buffer> repler::toBuffer()
{
    std::vector<asio::const_buffer> result {};
    result.push_back(asio::buffer(status_text(replyStatus_)));
    for (const headerLine & line : headerLines_)
    {
        result.push_back(asio::buffer(line.name));
        result.push_back(asio::buffer(http_protocol_str::name_value_glue));
        result.push_back(asio::buffer(line.value));
        result.push_back(asio::buffer(http_protocol_str::end_line));
    }
    result.push_back(asio::buffer(http_protocol_str::end_line));
    result.push_back(asio::buffer(content_));

    return result;
}
}
