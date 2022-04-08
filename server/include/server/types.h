#pragma once

#include <string>
#include <vector>
namespace sserver {
struct headerLine
{

    template<typename StrTypeName, typename StrTypeValue>
    headerLine (StrTypeName &&Aname, StrTypeValue &&Avalue) :
        name {std::forward<StrTypeName>(Aname)}, value {std::forward<StrTypeValue>(Avalue)} {}

    headerLine () = default;
    std::string name;
    std::string value;
};
using headers_t = std::vector<headerLine>;
}
