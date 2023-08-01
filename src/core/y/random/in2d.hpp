// \file

#ifndef Y_Random_In2D_Included
#define Y_Random_In2D_Included 1

#include "y/random/bits.hpp"
#include "y/mkl/numeric.hpp"

namespace Yttrium
{
    namespace Random
    {
        
        //! random (x,y) on the unit circle
        template <typename T>   inline
        void OnCircle(T &x, T &y, Random::Bits &ran) noexcept
        {
            const T theta = MKL::Numeric<T>::PI * ran.symm<float>();
            x = std::cos(theta);
            y = std::sin(theta);
        }

        //! random complex/v2d/writable...
        template <typename VTX> inline
        VTX OnCircle(Random::Bits &ran) noexcept
        { VTX v; OnCircle(v[1],v[2],ran); return v; }

        //! random (x,y) within the unit disk
        template <typename T> inline
        void InDisk(T &x, T &y, Random::Bits &ran) noexcept
        {
            static const T one(1);
            do {
                x = ran.symm<T>();
                y = ran.symm<T>();
            } while( x*x + y*y > one);
        }

        //! random complex/v2d/writable...
        template <typename VTX> inline
        VTX InDisk(Random::Bits &ran) noexcept
        { VTX v; InDisk(v[1],v[2],ran); return v; }



    }

}

#endif

