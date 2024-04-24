
#include "y/chemical/reactive/equilibrium/lua.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        LuaEquilibrium:: ~LuaEquilibrium() noexcept
        {
            
        }

        xreal_t LuaEquilibrium:: getK(real_t t)
        {
            return F(t);
        }
    }

}

