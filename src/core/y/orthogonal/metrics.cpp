
#include "y/orthogonal/metrics.hpp"

namespace Yttrium
{

    namespace Orthogonal
    {

        Metrics:: Metrics(const size_t n) noexcept :
        dimensions(n),
        concluding(dimensions-1)
        {
            assert(dimensions>0);
        }

        Metrics:: ~Metrics() noexcept
        {
        }

        Metrics:: Metrics(const Metrics &_) noexcept :
        dimensions(_.dimensions),
        concluding(_.concluding)
        {
        }

        Quality Metrics:: getQuality(const size_t n) const noexcept
        {
            assert(n>0);
            assert(n<=dimensions);
            if(n>=dimensions)
                return Generating;


            if(n>=concluding)
                return Hyperplane;

            return Fragmental;

        }


    }


}
