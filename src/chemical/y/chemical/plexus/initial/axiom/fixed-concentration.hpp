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
            class FixedConcentration : public Axiom
            {
            public:
                explicit FixedConcentration(const Species &sp,
                                            const xreal_t  C0) noexcept;
                virtual ~FixedConcentration() noexcept;



                const Species &species;

            private:
                Y_DISABLE_ASSIGN(FixedConcentration);
                FixedConcentration(const FixedConcentration &) noexcept;
                Y_Chemical_Axiom_Interface();
            };



            FixedConcentration:: FixedConcentration(const Species &sp,
                                                    const xreal_t  C0) noexcept :
            Axiom(C0),
            species(sp)
            {
            }

            FixedConcentration:: FixedConcentration(const FixedConcentration &_) noexcept :
            Axiom(_),
            species(_.species)
            {
            }

            FixedConcentration:: ~FixedConcentration() noexcept
            {
            }



            Axiom * FixedConcentration:: clone() const
            {
                return new FixedConcentration(*this);
            }

            int FixedConcentration:: weight(const Species &sp) const noexcept
            {
                return ( &sp == &species ) ? 1 : 0;
            }
        }

    }

}

#endif

