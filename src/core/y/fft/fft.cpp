
#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif

#if defined(__GNUC__)
#define _GNU_SOURCE
#endif

#include "y/fft/fft.hpp"
#include "y/text/human-readable.hpp"
#include "y/type/utils.hpp"



#include <iomanip>
#include <cmath>

namespace Yttrium
{

    static const uint64_t one64 = 1;

#define Y_FFT2(n)   ( one64<< (n) )
#define YFN(FN,n)   (FN(Y_FFT2(n)))
#define Y_FFT_ID(x) (x)

#define Y_FFT_REP(FN) \
YFN(FN,0),  YFN(FN,1),  YFN(FN,2),  YFN(FN,3),  YFN(FN,4),  YFN(FN,5),  YFN(FN,6),  YFN(FN,7),  YFN(FN,8),  YFN(FN,9),  \
YFN(FN,10), YFN(FN,11), YFN(FN,12), YFN(FN,13), YFN(FN,14), YFN(FN,15), YFN(FN,16), YFN(FN,17), YFN(FN,18), YFN(FN,19), \
YFN(FN,20), YFN(FN,21), YFN(FN,22), YFN(FN,23), YFN(FN,24), YFN(FN,25), YFN(FN,26), YFN(FN,27), YFN(FN,28), YFN(FN,29), \
YFN(FN,30), YFN(FN,31), YFN(FN,32), YFN(FN,33), YFN(FN,34), YFN(FN,35), YFN(FN,36), YFN(FN,37), YFN(FN,38), YFN(FN,39), \
YFN(FN,40), YFN(FN,41), YFN(FN,42), YFN(FN,43), YFN(FN,44), YFN(FN,45), YFN(FN,46), YFN(FN,47), YFN(FN,48), YFN(FN,49), \
YFN(FN,50), YFN(FN,51), YFN(FN,52), YFN(FN,53), YFN(FN,54), YFN(FN,55), YFN(FN,56), YFN(FN,57), YFN(FN,58), YFN(FN,59), \
YFN(FN,60), YFN(FN,61), YFN(FN,62), YFN(FN,63)



    template <typename T>
    struct _FFT
    {
        static const T PI;
        static const T two;
        static const T Fwd[];
        static const T Rev[];
        static const T Aux[];
    };


#define Y_FFT_SIN(denom) std::sin(PI/denom)
#define Y_FFT_AUX(denom) -two*Squared(std::sin(PI/denom) )

    template <> const double      _FFT<double>::PI     =  M_PI;
    template <> const double      _FFT<double>::two    =  2.0;
    template <> const double      _FFT<double>::Fwd [] = { Y_FFT_REP(Y_FFT_SIN)  };
    template <> const double      _FFT<double>::Rev [] = { Y_FFT_REP(-Y_FFT_SIN) };
    template <> const double      _FFT<double>::Aux [] = { Y_FFT_REP(Y_FFT_AUX)  };

    template <> const long double _FFT<long double>::PI     = static_cast<long double>(M_PI);
    template <> const long double _FFT<long double>::two    =  2.0l;
    template <> const long double _FFT<long double>::Fwd [] = { Y_FFT_REP(Y_FFT_SIN)  };
    template <> const long double _FFT<long double>::Rev [] = { Y_FFT_REP(-Y_FFT_SIN) };
    template <> const long double _FFT<long double>::Aux [] = { Y_FFT_REP(Y_FFT_AUX)  };

    size_t FFT:: CountXBR(const size_t size) noexcept
    {
        assert(IsPowerOfTwo(size));
        size_t       count = 0;
        const size_t half = size>>1;
        size_t j=0;
        for(size_t i=0;i<size;++i)
        {
            if(j>i)
                ++count;
            size_t m=half;
            while( (m>0) && j >= m)
            {
                j  -= m;
                m >>= 1;
            }
            j += m;
        }
        return count;
    }


    static FFT::XBR  XBRT[FFT::AvailableXBR];

    static inline
    const uint32_t *FillXBR(size_t      &offset,
                            const size_t size,
                            size_t      &count) noexcept
    {
        assert(offset<FFT::AvailableXBR);
        const uint32_t * ans  = (uint32_t *)&XBRT[offset];
        const size_t     half = size>>1;
        size_t j = 0;
        count    = 0;
        for(size_t i=0;i<size;++i)
        {
            if(j>i)
            {
                assert(offset<FFT::AvailableXBR);
                FFT::XBR &xbr = XBRT[offset++];
                xbr.i = i;
                xbr.j = j;
                ++count;
            }

            size_t m=half;
            while( (m>0) && j >= m)
            {
                j  -= m;
                m >>= 1;
            }
            j += m;
        }
        return ans;
    }



    const char * const FFT:: CallSign = "FFT";



    FFT:: FFT() 
    {
        if(Verbose)
        {
            std::cerr << "-- Initializing FFT Engine" << std::endl;
        }

        assert(64==sizeof(_FFT<double>::Fwd)/sizeof(double));
        assert(64==sizeof(_FFT<double>::Rev)/sizeof(double));
        assert(64==sizeof(_FFT<double>::Aux)/sizeof(double));

        assert(64==sizeof(_FFT<long double>::Fwd)/sizeof(long double));
        assert(64==sizeof(_FFT<long double>::Rev)/sizeof(long double));
        assert(64==sizeof(_FFT<long double>::Aux)/sizeof(long double));

        Y_STATIC_ZARR(XBRT);
        Y_STATIC_ZARR(xbrp);
        Y_STATIC_ZARR(xbrn);

        size_t offset=0;
        for(unsigned shift=MinShift; shift<=MaxShift; ++shift)
        {
            const size_t n = Base2<size_t>::One << shift;
            xbrp[shift] = FillXBR(offset,n,xbrn[shift])-1; assert(0!=xbrp[shift]);
            if(Verbose)
            {
                std::cerr << "-- 2^" << std::setw(2) << shift << " = " << std::setw(8) << n;
                std::cerr << " => #" << std::setw(8) << xbrn[shift];
                std::cerr << " => #" << std::setw(8) << offset;
                std::cerr << std::endl;
            }
        }

        
    }



    FFT:: ~FFT() noexcept
    {

    }



}

#define Real float
#include "fft.hxx"

#undef Real
#define Real double
#include "fft.hxx"

#undef Real
#define Real long double
#include "fft.hxx"



