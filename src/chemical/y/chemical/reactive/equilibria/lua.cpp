
#include "y/chemical/reactive/equilibria/lua.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        LuaEquilibria:: LuaEquilibria() : 
        Equilibria(),
        lvm( new Lua::State() )
        {}

        LuaEquilibria:: ~LuaEquilibria() noexcept
        {
        }
    }

}
