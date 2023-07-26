


#include "y/fft/fft.hpp"
#include "y/text/human-readable.hpp"
#include "y/type/utils.hpp"

#include <iomanip>

namespace Yttrium
{

    template <typename T>
    struct _FFT
    {
        static const T PI;
    };


    template <> const double       _FFT<double>::      PI = M_PI;
    template <> const long double  _FFT<long double>:: PI = static_cast<long double>(M_PI);

    
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


    const char * const FFT:: CallSign = "FFT";
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


    FFT:: FFT() 
    {
        if(Verbose)
        {
            std::cerr << "-- Initializing FFT Engine" << std::endl;
        }
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



