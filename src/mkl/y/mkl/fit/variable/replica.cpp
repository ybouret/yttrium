

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

            void ReplicaVariable:: dsp(std::ostream &os) const
            {
                const String &_ = *primary;
                os << _ << '@' << primary->idx();
            }

        }

    }

}
