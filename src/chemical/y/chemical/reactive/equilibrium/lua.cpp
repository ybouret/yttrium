
#include "y/chemical/reactive/equilibrium/lua.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        LuaEquilibrium:: ~LuaEquilibrium() noexcept
        {
            
        }

        XReal LuaEquilibrium:: getK(Real t)
        {
            return F(t);
        }
    }

}

