
//! \file

#ifndef Y_Chemical_Reactive_Army_Included
#define Y_Chemical_Reactive_Army_Included 1

#include "y/chem/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Army with different components
        //
        //
        //______________________________________________________________________
        class Army : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Army() noexcept; //!< setup
            virtual ~Army() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const EqRepo prodOnly; //!< with only (roaming) product  species
            const EqRepo reacOnly; //!< with only (roaming) reactant species
            const EqRepo spurious; //!< with both side of roaming species
            const EqRepo definite; //!< at least one conserved on each side

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Army);
        };
    }

}

#endif
