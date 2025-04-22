
//! \file


#ifndef Y_Chemical_Initial_Axiom_SteadyConservation_Included
#define Y_Chemical_Initial_Axiom_SteadyConservation_Included 1

#include "y/chemical/plexus/initial/axiom.hpp"
#include "y/chemical/plexus/conservation/law.hpp"

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
            //! Steady Conservation Axion
            //
            //
            //__________________________________________________________________
            class SteadyConservation : public Axiom
            {
            public:
                //! setup
                explicit SteadyConservation(const Conservation::Law &_law,
                                            const xreal_t            ctot) noexcept;

                //! cleanup
                virtual ~SteadyConservation() noexcept;

                //! raw to retrieve coefficients
                const Conservation::Law &law;

            private:
                Y_DISABLE_ASSIGN(SteadyConservation);
                SteadyConservation(const SteadyConservation &) noexcept;
                Y_Chemical_Axiom_Interface();
            };

        }

    }

}

#endif
