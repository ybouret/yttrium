
#include "y/chemical/plexus/initial/axiom/electroneutrality.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            ElectroNeutrality:: ~ElectroNeutrality() noexcept
            {
            }

            ElectroNeutrality:: ElectroNeutrality() noexcept :
            Axiom(0)
            {
            }


            int ElectroNeutrality:: weight(const Species &sp) const noexcept
            {
                return sp.z;
            }

            Axiom * ElectroNeutrality:: clone() const
            {
                return new ElectroNeutrality();
            }

        }

    }

}
