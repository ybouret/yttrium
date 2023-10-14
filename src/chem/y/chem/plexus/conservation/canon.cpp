
#include "y/chem/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Canon:: ~Canon() noexcept
        {
        }

        Canon:: Canon(const Conservation &first) :
        Object(), Conservations(), next(0), prev(0)
        {
            (*this) << first;
        }
    }

}
