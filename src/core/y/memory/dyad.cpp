#include "y/memory/dyad.hpp"
#include "y/calculus/base2.hpp"

namespace Yttrium
{

    namespace Memory
    {

        Dyad:: ~Dyad() noexcept
        {
        }

        Dyad:: Dyad(const unsigned userShift) noexcept :
        shift(userShift),
        bytes(Base2<size_t>::One << shift)
        {
            assert(shift<=Base2<size_t>::MaxShift);
        }
    }

}

