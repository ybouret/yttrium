
#include "y/mkl/fit/zero-bode.hpp"
#include "y/mkl/xreal.hpp"
#include "y/system/exception.hpp"
#include "y/container/cxx/array.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {
            ZeroBodeInfo::  ZeroBodeInfo() noexcept : Oversized() {}
            ZeroBodeInfo:: ~ZeroBodeInfo() noexcept {}

            void ZeroBodeInfo:: singular() const
            {
                throw Specific::Exception("ZeroBode", "singular extrapolation");
            }


        }

    }

}

#define real_t float
#include "zero-bode.hxx"
#undef real_t

#define real_t double
#include "zero-bode.hxx"
#undef real_t

#define real_t long double
#include "zero-bode.hxx"
#undef real_t

#define real_t XReal<float>
#include "zero-bode.hxx"
#undef real_t

#define real_t XReal<double>
#include "zero-bode.hxx"
#undef real_t

#define real_t XReal<long double>
#include "zero-bode.hxx"
#undef real_t
