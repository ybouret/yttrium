
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

        const Coord Edge:: Delta[8] =
        {
            Coord( 1, 0),
            Coord( 0, 1),
            Coord(-1, 0),
            Coord( 0,-1),

            Coord( 1, 1), // +x, +y
            Coord(-1, 1), // -x, +y
            Coord(-1,-1), // -x, -y
            Coord( 1,-1)  // +x, -y
        };
    }

}
