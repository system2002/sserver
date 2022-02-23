#pragma once

#include <functional>
#include "request.hpp"
#include "repler.h"
#include <unordered_map>



namespace sserver {


class handleManager
{


public:
    using handle_type = std::function<void(const request&, repler &)>;
    static handleManager & getManager();
    handleManager(const handleManager&) = delete;
    handleManager operator=(const handleManager&)  = delete;
    void callHandle(const request&, repler &) const;

    void insertEqual(std::string_view uriKey, handle_type &&handle);


private:
    using equalMap_type = std::unordered_map<std::string,  handle_type>;
    handleManager();
    equalMap_type m_equalHandle;
    equalMap_type m_prefixHandle;





};

}
