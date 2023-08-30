//! \file

#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED 1

#include "y/random/in2d.hpp"
#include "y/random/in3d.hpp"
#include "y/mkl/xreal.hpp"
#include "y/mkl/complex.hpp"
#include "y/mkl/v3d.hpp"
#include "y/apex/rational.hpp"
#include "y/string.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{

#define Y_BRING_MAXBITS 80

    template <typename T> struct Bring;

    template <>
    struct Bring<float>
    {
        static inline float Get(Random::Bits &ran) noexcept
        {
            return ran.symm<float>();
        }
    };

    template <>
    struct Bring<double>
    {
        static inline double Get(Random::Bits &ran) noexcept
        {
            return ran.symm<double>();
        }
    };

    template <>
    struct Bring<long double>
    {
        static inline long double Get(Random::Bits &ran) noexcept
        {
            return ran.symm<long double>();
        }
    };

    template <>
    struct Bring<uint16_t>
    {
        static inline uint16_t Get(Random::Bits &ran) noexcept
        {
            return ran.to<uint16_t>(unsigned(ran.leq(16)));
        }
    };


    template <typename T>
    struct Bring< XReal<T> >
    {
        static inline XReal<T> Get( Random::Bits &ran )
        {
            return Bring<T>::Get(ran);
        }
    };

    template <typename T>
    struct Bring< Complex<T> >
    {
        static inline Complex<T> Get( Random::Bits &ran )
        {
            return Complex<T>( Bring<T>::Get(ran), Bring<T>::Get(ran) );
        }
    };

    template <typename T>
    struct Bring< V2D<T> >
    {
        static inline V2D<T> Get(Random::Bits &ran)
        {
            return Random::InDisk< V2D<T> >(ran);
        }
    };

    template <typename T>
    struct Bring< V3D<T> >
    {
        static inline V3D<T> Get(Random::Bits &ran)
        {
            return Random::InBall< V3D<T> >(ran);
        }
    };


    template <>
    struct Bring< apn >
    {
        static inline apn Get(Random::Bits &ran)
        {
            return apn(ran.in<size_t>(0,Y_BRING_MAXBITS),ran);
        }
    };

    template <>
    struct Bring< apz >
    {
        static inline apz Get(Random::Bits &ran)
        {
            return apz(ran.in<size_t>(0,Y_BRING_MAXBITS),ran);
        }
    };

    template <>
    struct Bring< apq >
    {
        static inline apq Get(Random::Bits &ran)
        {
            return apq(ran.in<size_t>(0,Y_BRING_MAXBITS),ran.in<size_t>(1,Y_BRING_MAXBITS),ran);
        }
    };

    template <>
    struct Bring< String >
    {
        static inline String Get(Random::Bits &ran)
        {
            String s;
            for(size_t i=ran.in<size_t>(2,8);i>0;--i)
            {
                s << ran.in<char>('a','z');
            }
            return s;
        }
    };

    template <typename WRITABLE>
    inline void FillWritable(WRITABLE &data,
                             Random::Bits &ran)
    {
        for(size_t i=1;i<=data.size();++i)
            data[i] = Bring< typename WRITABLE::Type >::Get(ran);
    }

    template <typename T>
    inline void FillMatrix(Matrix<T>    &a,
                           Random::Bits &ran)
    {
        LightArray<T> arr = a.asArray();
        FillWritable(arr,ran);
    }

}

#endif

