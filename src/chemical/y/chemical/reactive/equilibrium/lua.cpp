
#include "y/chemical/reactive/equilibrium/lua.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        LuaEquilibrium:: ~LuaEquilibrium() noexcept
        {
        }

        xreal_t LuaEquilibrium:: getK(const xreal_t t)
        {
            return kfn( real_t(t) );
        }

    }

}

