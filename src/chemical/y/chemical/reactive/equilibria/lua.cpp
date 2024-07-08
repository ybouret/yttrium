
#include "y/chemical/reactive/equilibria/lua.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        LuaEquilibria:: ~LuaEquilibria() noexcept {}
        
        LuaEquilibria:: LuaEquilibria() : Equilibria(), lvm( new Lua::State() )
        {}

    }

}
