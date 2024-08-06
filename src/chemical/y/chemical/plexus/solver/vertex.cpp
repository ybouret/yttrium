
#include "y/chemical/plexus/solver/vertex.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        SignType Vertex::Comparator:: operator()(const Vertex * const lhs, const Vertex * const rhs) const noexcept
        {
            return Comparison::CxxDecreasing(lhs->cost, rhs->cost);
        }

        Vertex:: Vertex(const size_t species) :
        XArray(species), cost(0), next(0), prev(0)
        {

        }

        Vertex:: ~Vertex() noexcept {}

        void Vertex:: clear() noexcept { ld(cost=0); }
    }

}

