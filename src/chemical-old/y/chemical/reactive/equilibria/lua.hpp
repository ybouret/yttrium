
//! \file

#ifndef Y_Chemical_Equilibria_Lua_Included
#define Y_Chemical_Equilibria_Lua_Included 1

#include "y/chemical/reactive/equilibrium/lua.hpp"
#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Equilibrium with a Lua VM
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

            //! create an equilibrium with a function as constant
            template <typename EID, typename FID> inline
            Equilibrium & makeLua(const EID &eid, const FID &fid)
            {
                return insert( new LuaEquilibrium(eid,topLevel(),lvm,fid) );
            }

            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Lua::VM lvm; //!< lua (shared) virtual machine

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibria);
        };
    }

}

#endif

