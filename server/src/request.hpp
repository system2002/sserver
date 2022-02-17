#pragma once
#include <string>
#include <vector>
#include "types.h"

namespace sserver {

struct request
{
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<header> headers;

    bool headerContains(std::string_view key) const
    {
        for (const header & line : headers)
        {
            if (line.name == key) return true;
        }
        return false;
    }
    const std::string_view getValueHeaders(std::string_view key) const
    {
        for (auto it = headers.cbegin(); it != headers.cend(); ++it)
        {
            if (it->name == key) return it->value;
        }
        return std::string_view {};
    }

    void reset()
    {
        method.clear();
        uri.clear();
        headers.clear();
    }
};
}
