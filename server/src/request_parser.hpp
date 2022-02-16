#pragma once
namespace sserver {

struct request;

class request_parser
{
public:
    enum class result_t { good, bad, indeterminate };
    request_parser();
    void reset();

    template <typename InputIterator>
    result_t parse(request& request, InputIterator begin, InputIterator end)
    {
        while (begin != end)
        {
            result_t result = consume(request, *begin++);
            if (result == result_t::good || result == result_t::bad)
                return result;
        }
        return result_t::indeterminate;
    }

private:

    result_t consume(request& request, char input);
    static bool is_char(int c);
    static bool is_ctl(int c);
    static bool is_tspecial(int c);
    static bool is_digit(int c);

    enum class state_t
    {
        method_start,
        method,
        uri,
        http_version_h,
        http_version_t_1,
        http_version_t_2,
        http_version_p,
        http_version_slash,
        http_version_major_start,
        http_version_major,
        http_version_minor_start,
        http_version_minor,
        expecting_newline_1,
        header_line_start,
        header_lws,
        header_name,
        space_before_header_value,
        header_value,
        expecting_newline_2,
        expecting_newline_3
    };
    state_t m_state;
};

}
