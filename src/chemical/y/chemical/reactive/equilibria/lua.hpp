
//! \file

#ifndef Y_Chemical_Lua_Equilibria_Included
#define Y_Chemical_Lua_Equilibria_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/reactive/equilibrium/lua.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class LuaEquilibria : public Equilibria
        {
        public:
            explicit LuaEquilibria();
            virtual ~LuaEquilibria() noexcept;

            template <typename ID, typename FN> inline
            Equilibrium & newLua(const ID &name, const FN &kid)
            {
                return append( new LuaEquilibrium(name,db.size()+1,lvm,kid) );
            }

            Lua::VM lvm;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);
        };

    }

}

#endif

