
//! \file

#ifndef Y_Chemical_Reactive_Army_Included
#define Y_Chemical_Reactive_Army_Included 1

#include "y/chem/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Army : public Object
        {
        public:
            explicit Army() noexcept;
            virtual ~Army() noexcept;
            
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
