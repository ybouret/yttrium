
#include "y/chemical/plexus/initial/axiom.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            Axiom:: Axiom(const xreal_t value) noexcept :
            Quantized(),
            amount(value),
            next(0),
            prev(0)
            {
            }

            Axiom:: Axiom(const Axiom &_) noexcept :
            Quantized(),
            amount(_.amount),
            next(0),
            prev(0)
            {
            }


            Axiom:: ~Axiom() noexcept
            {
            }
        }
    }
}
