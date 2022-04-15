#pragma once

#include <functional>
#include <unordered_map>
#include "server/request.hpp"
#include "server/repler.h"



namespace sserver {


class handleManager
{


public:
    using handle_type = std::function<void(const request&, repler &)>;
    static handleManager & getManager();
    handleManager(const handleManager&) = delete;
    handleManager operator=(const handleManager&)  = delete;
    void callHandle(const request&, repler &) const;

    void appendEqualHandle(std::string_view uriKey, handle_type &&handle);
    void appendPrefixHandle(std::string_view uriKey, handle_type &&handle);


private:
    using handleMap_type = std::unordered_map<std::string,  handle_type>;
    handleManager();
    handleMap_type m_equalHandle;
    handleMap_type m_prefixHandle;

};

}
