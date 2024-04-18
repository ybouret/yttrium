
//! \file

#ifndef Y_Chemical_Equilibrium_Lua_Included
#define Y_Chemical_Equilibrium_Lua_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/lua++/function.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class LuaEquilibrium : public Equilibrium
        {
        public:
            virtual ~LuaEquilibrium() noexcept;

            template <typename UID, typename FID> inline
            explicit LuaEquilibrium(const UID    &uid,
                                    const size_t  topLevel,
                                    const Lua::VM &lvm,
                                    const FID    &fid) :
            Equilibrium(uid,topLevel),
            F(lvm,fid)
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibrium);
            Lua::Function<Real> F;
            virtual XReal getK(Real t);
        };
    }

}

#endif

