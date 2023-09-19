
#include "y/apex/ortho/metrics.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {

            Metrics:: Metrics(const size_t n) noexcept :
            dimensions(n)
            {
                assert(dimensions>0);
            }

            Metrics:: ~Metrics() noexcept
            {
            }

            Metrics:: Metrics(const Metrics &_) noexcept :
            dimensions(_.dimensions)
            {
            }
            

        }

    }

}
