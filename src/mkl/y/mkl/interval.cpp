
#include "y/mkl/interval.hpp"

namespace Yttrium
{

    namespace MKL
    {
        Y_SHALLOW_IMPL(Infinity);
        
        namespace Kernel
        {
            Interval::  Interval() noexcept {}
            Interval:: ~Interval() noexcept {}

            const char Interval:: INFTY[] = "oo";
        }

    }

}

