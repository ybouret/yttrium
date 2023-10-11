#include "y/lua++/function.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Lua
    {
        Function_:: ~Function_() noexcept
        {
        }

        Function_:: Function_(const Function_ &F) :
        VM(F),
        name(F.name)
        {
        }
        
        void Function_:: fetch()
        {
            lua_State *L = ***this;
            lua_settop(L,0);
            lua_getglobal(L,name());
        }

        void Function_::call(int nargs)
        {
            lua_State *L = ***this;

            /* do the call (2 arguments, 1 result) */
            if (lua_pcall(L, nargs, 1, 0) != 0)
                throw Exception("call '%s': %s",name(), lua_tostring(L, -1));

            /* retrieve result */
            if (!lua_isnumber(L, -1))
                throw Exception("function '%s' must return a number, no %s", name(), luaL_typename(L,-1));
        }

    }

}

