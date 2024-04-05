
#include "y/mkl/tao/par/driver1d.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {
            Driver1D:: ~Driver1D() noexcept
            {
            }

            Driver1D:: Driver1D(const ThreadContext &ctx) noexcept :
            Concurrent::Frame1D<size_t>(ctx),
            XAddPtr()
            {
            }
            
        }

    }

}
