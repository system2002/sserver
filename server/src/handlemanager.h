#pragma once

#include <functional>
#include "server/request.hpp"
#include "server/repler.h"
#include <unordered_map>
#include <map>
#include <unordered_set>



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
//    std::map<size_t, handleMap_type> m_prefixSize;





};

}
