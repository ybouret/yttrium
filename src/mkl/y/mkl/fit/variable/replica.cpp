

#include "y/mkl/fit/variable/replica.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {
            ReplicaVariable:: ~ReplicaVariable() noexcept
            {
            }

            size_t ReplicaVariable:: idx() const noexcept
            {
                return primary->idx();
            }
        }

    }

}
