

#include "y/mkl/tao/par/driver2d.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {
            Driver2D:: ~Driver2D() noexcept
            {
            }

            Driver2D:: Driver2D(const ThreadContext &ctx) noexcept :
            Concurrent::Frame2D<size_t>(ctx),
            XAddPtr(),
            strip( *(const Strip **) Memory::OutOfReach::Addr(&segment) )
            {
            }

        }

    }

}
