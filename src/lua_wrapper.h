#pragma once

#include "../lua-5.4.6/src/lua.hpp"
#include <iostream>
#include <vector>
#include <string>

// This declaration must be seen by result.h and the instantiation of Result<std::vector<std::string>,...>.
// Need to find a less hacky solution for this.
std::ostream &operator<<(std::ostream &stream, const std::vector<std::string> &v);

#include "result.h"


namespace pul
{

    enum class LuaError
    {
        FileOpenError,
        StringNotFound,
        StringParseError,
        NumberNotFound,
        TableNotFound,
    };

    std::ostream &operator<<(std::ostream &stream, const LuaError &e);

    class __declspec(dllexport) LuaInstance
    {
    public:
        using StringResult = Result<const char *, LuaError>;
        using NumberResult = Result<double, LuaError>;
        using TableResult = Result<std::vector<std::string>, LuaError>;

        LuaInstance();
        LuaInstance(const LuaInstance &) = delete;
        LuaInstance operator=(const LuaInstance &) = delete;
        ~LuaInstance();

        StringResult exec_file(const char *fn);

        StringResult get_string(const char *var_name);
        NumberResult get_number(const char *var_name);
        TableResult get_table(const char *var_name);

    private:
        lua_State *const m_L;
    };

}
