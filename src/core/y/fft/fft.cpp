
#include "y/fft/fft.hpp"
#include "y/memory/pages.hpp"
#include "y/text/human-readable.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/counting/partition.hpp"

#include <iomanip>

namespace Yttrium
{

    static inline
    size_t CountXBR(const size_t size) noexcept
    {
        size_t       count = 0;
        const size_t half = size>>1;
        size_t j=0;
        for(size_t i=0;i<size;++i)
        {
            if(j>i)
            {
                //Swap(data[j],data[i]);
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
        return count;
    }



    const char * const FFT:: CallSign = "FFT";


    namespace
    {
        static const unsigned   BytesPerXBR  = sizeof(FFT::XBR);
        static const size_t     TableBytes   = 65536;
        static const size_t     AvailableXBR = TableBytes/BytesPerXBR;
        static FFT::XBR         XBRT[AvailableXBR];
    }

    static inline
    const FFT::XBR *FillXBR(size_t &offset, const size_t size) noexcept
    {
        assert(offset<AvailableXBR);
        const FFT::XBR   *ans  = &XBRT[offset];
        const size_t half = size>>1;
        size_t j=0;
        for(size_t i=0;i<size;++i)
        {
            if(j>i)
            {
                //Swap(data[j],data[i]);
                //++count;
                assert(offset<AvailableXBR);
                FFT::XBR &xbr = XBRT[offset++];
                xbr.i = i;
                xbr.j = j;
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


    FFT:: FFT() :
    xbrMin(0),
    xbrMax(0)
    {
        static const size_t  one = 1;

        std::cerr << "-- Initializing FFT Engine" << std::endl;
        std::cerr << "-- BytesPerXBR   = " << std::setw(8) << BytesPerXBR  << " bytes" << std::endl;
        std::cerr << "-- AvailableXBR  = " << std::setw(8) << AvailableXBR << " swaps" << std::endl;


        unsigned minShift = 0;
        unsigned maxShift = 0;
        size_t   totalXBR = 0;
        for(unsigned shift=0;;++shift)
        {
            const size_t n = one << shift;
            const size_t x = CountXBR(n);
            if(x>0&&minShift<=0) { maxShift = minShift = shift; }
            const size_t newTotal = x+totalXBR;
            if(newTotal>AvailableXBR) break;
            maxShift = shift;
            totalXBR = newTotal;
            std::cerr << "-- 2^" << std::setw(2) << shift << " = " << std::setw(8) << n << " => " << x << " => " << totalXBR << std::endl;
        }

        std::cerr << "-- minShift : 2^" << std::setw(2) << minShift << std::endl;
        std::cerr << "-- maxShift : 2^" << std::setw(2) << maxShift << std::endl;
        Coerce(xbrMin) = one << minShift;
        Coerce(xbrMax) = one << maxShift;

        size_t offset = 0;
        for(unsigned shift=minShift;shift<=maxShift;++shift)
        {
            FillXBR(offset,one<<shift);
        }





    }



    FFT:: ~FFT() noexcept
    {

    }
    

}


