
#include "y/chemical/plexus/initial/axiom.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            Axiom:: Axiom(const xreal_t value, const uint32_t t) noexcept :
            Quantized(),
            amount(value),
            uuid(t),
            next(0),
            prev(0)
            {
            }

            Axiom:: Axiom(const Axiom &_) noexcept :
            Quantized(),
            amount(_.amount),
            uuid(_.uuid),
            next(0),
            prev(0)
            {
            }


            Axiom:: ~Axiom() noexcept
            {
            }

            bool Axiom:: contains(const Species &sp) const noexcept
            {
                return 0 != weight(sp);
            }
        }
    }
}
