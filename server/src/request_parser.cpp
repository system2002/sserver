#include "request_parser.hpp"
#include "server/request.hpp"


namespace sserver {

request_parser::request_parser()
    : m_state(state_t::method_start)
{}

void request_parser::reset()
{
    m_state = state_t::method_start;
}

request_parser::result_t request_parser::consume(request& req, char input)
{
    switch (m_state)
    {
    case state_t::method_start:
        if (!is_char(input) || is_ctl(input) || is_tspecial(input))
        {
            return result_t::bad;
        }
        else
        {
            m_state = state_t::method;
            req.method.push_back(input);
            return result_t::indeterminate;
        }
    case state_t::method:
        if (input == ' ')
        {
            m_state = state_t::uri;
            return result_t::indeterminate;
        }
        else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
        {
            return result_t::bad;
        }
        else
        {
            req.method.push_back(input);
            return result_t::indeterminate;
        }
    case state_t::uri:
        if (input == ' ')
        {
            m_state = state_t::http_version_h;
            return result_t::indeterminate;
        }
        else if (is_ctl(input))
        {
            return result_t::bad;
        }
        else
        {
            req.uri.push_back(input);
            return result_t::indeterminate;
        }
    case state_t::http_version_h:
        if (input == 'H')
        {
            m_state = state_t::http_version_t_1;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::http_version_t_1:
        if (input == 'T')
        {
            m_state = state_t::http_version_t_2;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::http_version_t_2:
        if (input == 'T')
        {
            m_state = state_t::http_version_p;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::http_version_p:
        if (input == 'P')
        {
            m_state = state_t::http_version_slash;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::http_version_slash:
        if (input == '/')
        {
            req.http_version_major = 0;
            req.http_version_minor = 0;
            m_state = state_t::http_version_major_start;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::http_version_major_start:
        if (is_digit(input))
        {
            req.http_version_major = req.http_version_major * 10 + input - '0';
            m_state = state_t::http_version_major;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::http_version_major:
        if (input == '.')
        {
            m_state = state_t::http_version_minor_start;
            return result_t::indeterminate;
        }
        else if (is_digit(input))
        {
            req.http_version_major = req.http_version_major * 10 + input - '0';
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::http_version_minor_start:
        if (is_digit(input))
        {
            req.http_version_minor = req.http_version_minor * 10 + input - '0';
            m_state = state_t::http_version_minor;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::http_version_minor:
        if (input == '\r')
        {
            m_state = state_t::expecting_newline_1;
            return result_t::indeterminate;
        }
        else if (is_digit(input))
        {
            req.http_version_minor = req.http_version_minor * 10 + input - '0';
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::expecting_newline_1:
        if (input == '\n')
        {
            m_state = state_t::header_line_start;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::header_line_start:
        if (input == '\r')
        {
            m_state = state_t::expecting_newline_3;
            return result_t::indeterminate;
        }
        else if (!req.headers.empty() && (input == ' ' || input == '\t'))
        {
            m_state = state_t::header_lws;
            return result_t::indeterminate;
        }
        else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
        {
            return result_t::bad;
        }
        else
        {
            req.headers.push_back(headerLine());
            req.headers.back().name.push_back(input);
            m_state = state_t::header_name;
            return result_t::indeterminate;
        }
    case state_t::header_lws:
        if (input == '\r')
        {
            m_state = state_t::expecting_newline_2;
            return result_t::indeterminate;
        }
        else if (input == ' ' || input == '\t')
        {
            return result_t::indeterminate;
        }
        else if (is_ctl(input))
        {
            return result_t::bad;
        }
        else
        {
            m_state = state_t::header_value;
            req.headers.back().value.push_back(input);
            return result_t::indeterminate;
        }
    case state_t::header_name:
        if (input == ':')
        {
            m_state = state_t::space_before_header_value;
            return result_t::indeterminate;
        }
        else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
        {
            return result_t::bad;
        }
        else
        {
            req.headers.back().name.push_back(input);
            return result_t::indeterminate;
        }
    case state_t::space_before_header_value:
        if (input == ' ')
        {
            m_state = state_t::header_value;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::header_value:
        if (input == '\r')
        {
            m_state = state_t::expecting_newline_2;
            return result_t::indeterminate;
        }
        else if (is_ctl(input))
        {
            return result_t::bad;
        }
        else
        {
            req.headers.back().value.push_back(input);
            return result_t::indeterminate;
        }
    case state_t::expecting_newline_2:
        if (input == '\n')
        {
            m_state = state_t::header_line_start;
            return result_t::indeterminate;
        }
        else
        {
            return result_t::bad;
        }
    case state_t::expecting_newline_3:
        return (input == '\n') ? result_t::good : result_t::bad;
    default:
        return result_t::bad;
    }
}

bool request_parser::is_char(int c)
{
    return c >= 0 && c <= 127;
}

bool request_parser::is_ctl(int c)
{
    return (c >= 0 && c <= 31) || (c == 127);
}

bool request_parser::is_tspecial(int c)
{
    switch (c)
    {
    case '(': case ')': case '<': case '>': case '@':
    case ',': case ';': case ':': case '\\': case '"':
    case '/': case '[': case ']': case '?': case '=':
    case '{': case '}': case ' ': case '\t':
        return true;
    default:
        return false;
    }
}

bool request_parser::is_digit(int c)
{
    return c >= '0' && c <= '9';
}

}

