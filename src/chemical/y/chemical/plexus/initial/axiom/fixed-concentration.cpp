
#include "y/chemical/plexus/initial/axiom/fixed-concentration.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            FixedConcentration:: FixedConcentration(const Species &sp,
                                                    const xreal_t  C0) noexcept :
            Axiom(C0,UUID),
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

