
#include "y/chemical/plexus/initial/axiom/steady-conservation.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            SteadyConservation:: SteadyConservation(const Conservation::Law &_law,
                                                    const xreal_t            ctot) noexcept :
            Axiom(ctot,UUID),
            law(_law)
            {

            }

            SteadyConservation:: SteadyConservation(const SteadyConservation &_) noexcept :
            Axiom(_),
            law(_.law)
            {
            }
            

            SteadyConservation:: ~SteadyConservation() noexcept
            {
            }

            Axiom * SteadyConservation:: clone() const {
                return new SteadyConservation(*this);
            }

            int SteadyConservation:: weight(const Species &sp) const noexcept
            {
                for(const Actor *a=law->head;a;a=a->next)
                {
                    if( &sp == &a->sp) return int(a->nu);
                }
                
                return 0;
            }

        }
    }

}

