#include "lua_wrapper.h"

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
            default:
                return "LUA ERROR: Unknown error";
            }
        };

        return stream << output(error);
    }
}