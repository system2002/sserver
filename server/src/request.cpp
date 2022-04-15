#include <algorithm>
#include "server/request.hpp"

namespace sserver {

bool request::headerContains(std::string_view key) const
{
    return std::any_of(headers.cbegin(), headers.cend(), [key](const headerLine & line)
    {
        return line.name == key;
    });
}

std::string_view request::getValueHeaders(std::string_view key) const
{
    auto find = std::find_if(headers.cbegin(), headers.cend(), [key](const headerLine & line)
    {
        return line.name == key;
    });
    if (find != headers.cend())
    {
        return find->value;
    }
    return std::string_view {};
}

}
