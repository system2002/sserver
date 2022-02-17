#pragma once

#include <string>
#include <vector>
namespace sserver {
struct header
{

    template<typename StrTypeName, typename StrTypeValue>
    header (StrTypeName Aname, StrTypeValue Avalue) :
        name {std::forward<StrTypeName>(Aname)}, value {std::forward<StrTypeValue>(Avalue)} {}

    header () {}
    std::string name;
    std::string value;
};
using headers_t = std::vector<header>;
}
