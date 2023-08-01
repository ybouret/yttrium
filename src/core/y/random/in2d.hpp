// \file

#ifndef Y_Random_In2D_Included
#define Y_Random_In2D_Included 1

#include "y/random/bits.hpp"
#include "y/mkl/numeric.hpp"

namespace Yttrium
{
    namespace Random
    {
        //______________________________________________________________________
        //
        //
        //! filling with random content
        //
        //______________________________________________________________________
        struct In2D
        {
            template <typename T>
            static void Circle(T &x, T &y, Random::Bits &ran) noexcept
            {
                const T theta = MKL::Numeric<T>::PI * ran.symm<float>();
                x = std::cos(theta);
                y = std::sin(theta);
            }

            template <typename T>
            static void Disk(T &x, T &y, Random::Bits &ran) noexcept
            {
                static const T one(1);
                do {
                    x = ran.symm<T>();
                    y = ran.symm<T>();
                } while( x*x + y*y > one);
            }
        };

    }

}

#endif

