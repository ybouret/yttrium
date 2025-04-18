//! \file


#ifndef Y_Chemical_Initial_Axiom_ElectroNeutrality_Included
#define Y_Chemical_Initial_Axiom_ElectroNeutrality_Included 1

#include "y/chemical/plexus/initial/axiom.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            //__________________________________________________________________
            //
            //
            //
            //! Electroneutrality Axion
            //
            //
            //__________________________________________________________________
            class ElectroNeutrality : public Axiom
            {
            public:
                explicit ElectroNeutrality() noexcept; //!< setup
                virtual ~ElectroNeutrality() noexcept; //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ElectroNeutrality);
                Y_Chemical_Axiom_Interface();
            };


        }

    }

}

#endif

