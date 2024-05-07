
#include "y/ink/ops/edge.hpp"

namespace Yttrium
{
    namespace Ink
    {

        Edge:: ~Edge() noexcept {}
        
        Edge:: Edge(const size_t i, const CoordBank &bank) :
        CoordList(bank),
        label(i),
        next(0),
        prev(0)
        {
        }

    }

}
