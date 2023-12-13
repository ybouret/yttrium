#include "y/concurrent/resources.hpp"
#include "y/system/exception.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Concurrent
    {


        namespace Nucleus
        {
            Resources:: ~Resources() noexcept
            {
            }

            void Resources:: throwBadLocus2D() const
            {
                throw Libc::Exception(EDOM,"Invalid 2D Locus");
            }
        }

    }

}


