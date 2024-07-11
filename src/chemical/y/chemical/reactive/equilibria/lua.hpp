
//! \file

#ifndef Y_Chemical_Lua_Equilibria_Included
#define Y_Chemical_Lua_Equilibria_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/reactive/equilibrium/lua.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Equilibria with Lua virtual machine for computations
        //
        //
        //______________________________________________________________________
        class LuaEquilibria : public Equilibria
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit LuaEquilibria();           //!< setup
            virtual ~LuaEquilibria() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
          
            //! create a new LuaEquilbrium
            template <typename ID, typename FN> inline
            Equilibrium & newLua(const ID &name, const FN &kid)
            {
                return append( new LuaEquilibrium(name,db.size()+1,lvm,kid) );
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Lua::VM lvm; //!< lua virtual machine

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);
        };

    }

}

#endif

