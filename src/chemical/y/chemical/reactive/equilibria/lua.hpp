
//! \file

#ifndef Y_Chemical_Lua_Equilibria_Included
#define Y_Chemical_Lua_Equilibria_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/lua++/state.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class LuaEquilibria : public Equilibria
        {
        public:
            explicit LuaEquilibria();
            virtual ~LuaEquilibria() noexcept;

            Lua::VM lvm;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);
        };

    }

}

#endif

