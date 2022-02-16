#pragma once
#include <string>
#include <vector>

namespace sserver {
struct header
{
  std::string name;
  std::string value;
};

struct request
{
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<header> headers;
    void reset()
    {
        method.clear();
        uri.clear();
        headers.clear();
    }
};
}
