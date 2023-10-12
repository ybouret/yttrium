
//! \file

#ifndef Y_Chemical_Lua_Equilibria_Included
#define Y_Chemical_Lua_Equilibria_Included 1

#include "y/chem/reactive/equilibria.hpp"
#include "y/lua++/state.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Equilibria with a Lua Virtual Machine inside
        //
        //
        //______________________________________________________________________
        class LuaEquilibria : public Equilibria
        {
        public:
            explicit LuaEquilibria();          //!< setup
            virtual ~LuaEquilibria() noexcept; //!< cleanup
            Lua::VM vm;                        //!< shared lua state

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);
        };
    }

}


#endif
