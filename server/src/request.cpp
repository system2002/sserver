#include <algorithm>
#include "server/request.hpp"

namespace sserver {

bool request::headerContains(std::string_view key) const
{
    return headers.cend() == std::find_if(headers.cbegin(), headers.cend(), [key](const headerLine & line)
    {
        return line.name == key;
    });

//    for (const headerLine & line : headers)
//    {
//        if (line.name == key) return true;
//    }
//    return false;
}

std::string_view request::getValueHeaders(std::string_view key) const
{
    for (auto it = headers.cbegin(); it != headers.cend(); ++it)
    {
        if (it->name == key) return it->value;
    }
    return std::string_view {};
}

}
