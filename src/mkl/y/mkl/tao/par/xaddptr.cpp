
#include "y/mkl/tao/par/xaddptr.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            XAddPtr:: XAddPtr() noexcept : handle(0) {}

            XAddPtr:: ~XAddPtr() noexcept
            {
            }

            void XAddPtr:: unlink() noexcept { handle=0; }
        }

    }
}
