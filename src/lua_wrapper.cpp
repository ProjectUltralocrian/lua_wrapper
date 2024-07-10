#include "lua_wrapper.h"
#include <string>

std::ostream &operator<<(std::ostream &stream, const std::vector<std::string> &v)
{
    for (const auto &s : v)
    {
        stream << "  - " << s << std::endl;
    }
    return stream;
}

namespace pul
{
    LuaInstance::LuaInstance()
        : m_L{luaL_newstate()}
    {
        luaL_openlibs(m_L);
    }

    LuaInstance::~LuaInstance()
    {
        lua_close(m_L);
    }

    LuaInstance::StringResult LuaInstance::exec_file(const char *fn)
    {
        auto result = luaL_loadfile(m_L, fn);
        if (result != 0)
        {
            std::cout << lua_tostring(m_L, -1) << std::endl;
            lua_pop(m_L, 1); // check if this is needed
            return LuaInstance::StringResult::Error(LuaError::FileOpenError);
        }

        lua_pcall(m_L, 0, LUA_MULTRET, 0);
        return LuaInstance::StringResult::Ok("File successfully executed.");
    }

    LuaInstance::StringResult LuaInstance::get_string(const char *var_name)
    {
        auto result = lua_getglobal(m_L, var_name);
        if (result != LUA_TSTRING)
        {
            lua_pop(m_L, 1);
            return LuaInstance::StringResult::Error(LuaError::StringNotFound);
        }
        auto s = LuaInstance::StringResult::Ok(lua_tostring(m_L, -1));
        lua_pop(m_L, 1);
        return s;
    }
    LuaInstance::NumberResult LuaInstance::get_number(const char *var_name)
    {
        auto result = lua_getglobal(m_L, var_name);
        if (result != LUA_TNUMBER)
        {
            lua_pop(m_L, 1);
            return LuaInstance::NumberResult::Error(LuaError::NumberNotFound);
        }
        auto s = LuaInstance::NumberResult::Ok(lua_tonumber(m_L, -1));
        lua_pop(LuaInstance::m_L, 1);
        return s;
    }

    LuaInstance::TableResult LuaInstance::get_all_values_in_table(const char *var_name)
    {
        auto result = lua_getglobal(m_L, var_name);
        if (result != LUA_TTABLE)
        {
            lua_pop(m_L, 1);
            return LuaInstance::TableResult::Error(LuaError::TableNotFound);
        }

        lua_pushnil(m_L);
        std::vector<std::string> output{};
        while (lua_next(m_L, -2))
        {
            if (lua_isstring(m_L, -1))
            {
                auto s = lua_tostring(m_L, -1);
                output.push_back(std::string(s));
            }
            lua_pop(m_L, 1);
        }
        lua_pop(m_L, 1);
        return TableResult::Ok(output);
    }

    std::ostream &operator<<(std::ostream &stream, const LuaError &error)
    {
        auto output = [](const LuaError &e)
        {
            switch (e)
            {
            case LuaError::FileOpenError:
                return "LUA ERROR: Error opening file.";
            case LuaError::StringNotFound:
                return "LUA ERROR: String not found.";
            case LuaError::StringParseError:
                return "LUA ERROR: String could not be parsed.";
            case LuaError::NumberNotFound:
                return "LUA ERROR: Number not found.";
            case LuaError::TableNotFound:
                return "LUA ERROR: Table not found.";
            default:
                return "LUA ERROR: Unknown error...";
            }
        };

        return stream << output(error);
    }
}