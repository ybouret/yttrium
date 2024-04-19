
//! \file

#ifndef Y_Chemical_Equilibria_Lua_Included
#define Y_Chemical_Equilibria_Lua_Included 1

#include "y/chemical/reactive/equilibrium/lua.hpp"
#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class LuaEquilibria : public Equilibria
        {
        public:
            explicit LuaEquilibria();
            virtual ~LuaEquilibria() noexcept;

            template <typename NAME> inline
            Equilibrium & operator()(const NAME &name, const String &fid)
            {
                return insert( new LuaEquilibrium(name,level(),lvm,fid) );
            }

            Lua::VM lvm;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);
        };
    }

}

#endif

