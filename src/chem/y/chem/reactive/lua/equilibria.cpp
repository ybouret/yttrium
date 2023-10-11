
#include "y/chem/reactive/lua/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        LuaEquilibria:: ~LuaEquilibria() noexcept
        {
        }

        LuaEquilibria:: LuaEquilibria() : Equilibria(), vm( new Lua::State() )
        {
        }
        

    }

}
