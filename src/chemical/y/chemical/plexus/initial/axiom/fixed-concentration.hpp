//! \file


#ifndef Y_Chemical_Initial_Axiom_FixedConcentration_Included
#define Y_Chemical_Initial_Axiom_FixedConcentration_Included 1

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
            //! Fixed concentration Axiom
            //
            //
            //__________________________________________________________________
            class FixedConcentration : public Axiom
            {
            public:
                //! setup
                explicit FixedConcentration(const Species &sp,
                                            const xreal_t  C0) noexcept;
                virtual ~FixedConcentration() noexcept; //!< cleanup


                const Species &species; //!< species with concentration to fix

            private:
                Y_DISABLE_ASSIGN(FixedConcentration);
                FixedConcentration(const FixedConcentration &) noexcept;
                Y_Chemical_Axiom_Interface();
            };



        }

    }

}

#endif

