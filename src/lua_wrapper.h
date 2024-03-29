#pragma once

#include "result.h"
#include "../lua-5.4.6/src/lua.hpp"
#include <iostream>

namespace pul
{

    enum class LuaError
    {
        FileOpenError,
        StringNotFound,
        StringParseError,
        NumberNotFound,
    };

    std::ostream &operator<<(std::ostream &stream, const LuaError &e);

    class LuaInstance
    {
    public:
        using StringResult = Result<std::string, LuaError>;
        using NumberResult = Result<double, LuaError>;

        LuaInstance();
        ~LuaInstance();

        StringResult exec_file(const char *fn);

        StringResult get_string(const char *var_name);
        NumberResult get_number(const char *var_name);

    private:
        lua_State *const m_L;
    };

}
