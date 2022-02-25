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
    std::vector<headerLine> headers;

    bool headerContains(std::string_view key) const;
    std::string_view getValueHeaders(std::string_view key) const;

    void reset()
    {
        method.clear();
        uri.clear();
        headers.clear();
    }
};
}
