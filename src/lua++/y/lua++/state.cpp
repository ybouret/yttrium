
#include "y/lua++/state.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Lua
    {
        const char State:: dummy_name[] = "dummy_name";

        State:: ~State() noexcept
        {
            lua_close(L);
        }

        State:: State() :
        L( luaL_newstate() )
        {
            if(!L) throw Exception("luaL_newstate failure");
            luaL_openlibs(L);
        }

        lua_State * State:: operator*() noexcept
        {
            assert(L);
            return L;
        }


        void State:: getglobal(const String &name)
        {
            lua_settop(L,0);
            lua_getglobal(L,name());
            
        }

        void State:: getglobal(const char   *name)
        {
            const String _(name);
            return getglobal(_);
        }

        void State:: dofile(const String &filename)
        {
            if(LUA_OK!=luaL_dofile(L,filename()))
            {
                throw Exception("luaL_dofile(%s): %s",filename(),lua_tostring(L,-1));
            }
        }

        void  State:: dostring(const String &buffer)
        {
            if(LUA_OK!=luaL_dostring(L,buffer()))
            {
                throw Exception("luaL_dostring: %s",lua_tostring(L,-1));
            }
        }


        void  State:: dostring(const char *buffer)
        {
            const String _(buffer);
            dostring(_);
        }


        void State:: dofile(const char   *filename)
        {
            const String _(filename);
            dofile(_);
        }


        lua_Integer State:: to(const Type2Type<lua_Integer> &, const int idx)
        {
            if(!lua_isinteger(L,idx))
            {
                throw Exception("lua_isinteger got %s",luaL_typename(L,idx));
            }
            return lua_tointeger(L,idx);
        }

        lua_Number State:: to(const Type2Type<lua_Number> &, const int idx)
        {
            if(!lua_isnumber(L,idx))
            {
                throw Exception("lua_isnumber got %s",luaL_typename(L,idx));
            }
            return lua_tonumber(L,idx);
        }

        String State::to(const Type2Type<String> &, const int idx)
        {
            if(!lua_isstring(L,idx))
            {
                throw Exception("lua_isstring got %s",luaL_typename(L,idx));
            }
            size_t      len = 0;
            const char *str = lua_tolstring(L,idx,&len);
            return String(str,len);
        }

        void State:: set_dummy_name(const String &expr)
        {
            const String code = dummy_name + ( '=' + expr);
            dostring(code);
        }

        void State:: set_dummy_name(const char *expr)
        {
            const String _(expr);
            set_dummy_name(_);
        }


        int State:: type(const int idx) noexcept
        {
            return lua_type(L,idx);
        }
    }
}
