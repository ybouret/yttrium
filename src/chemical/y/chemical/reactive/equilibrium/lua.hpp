
//! \file

#ifndef Y_Chemical_Equilibrium_Lua_Included
#define Y_Chemical_Equilibrium_Lua_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/lua++/function.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Equilibrium with a Lua function as constant
        //
        //
        //______________________________________________________________________
        class LuaEquilibrium : public Equilibrium
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            
            //! setup with name, index and lua function name
            template <typename ID, typename FN>
            explicit LuaEquilibrium(const ID    &userName,
                                    const size_t topLevel,
                                    const Lua::VM &lvm,
                                    const FN      &fid) :
            Equilibrium(userName,topLevel),
            kfn(lvm,fid)
            {
            }

            //! cleanup
            virtual ~LuaEquilibrium() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibrium);
            Lua::Function<real_t> kfn;
            virtual xreal_t getK(const xreal_t);
        };

    }

}


#endif
