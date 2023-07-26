
#include "y/fft/fft.hpp"
#include "y/memory/pages.hpp"
#include "y/text/human-readable.hpp"
#include "y/type/utils.hpp"
#include "y/container/cxx-array.hpp"
#include "y/memory/allocator/pooled.hpp"

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
                //jmax = Max(jmax,j);
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

    FFT:: FFT() :
    xbrMin(0),
    xbrMax(0)
    {
        static const size_t  one = 1;
        typedef uint16_t     IndexType;
        static  const size_t IndexSize = sizeof(IndexType);
        static  const size_t TableSize = Min<size_t>(Memory::Page::DefaultBytes,size_t(IntegerFor<IndexType>::Maximum)+1);
        static  const size_t Available = TableSize / IndexSize;

        std::cerr << "-- Initializing FFT Engine" << std::endl;
        std::cerr << "-- IndexSize = " << std::setw(8) << IndexSize << " bytes" << std::endl;
        std::cerr << "-- TableSize = " << std::setw(8) << TableSize << " bytes" << std::endl;
        std::cerr << "-- Available = " << std::setw(8) << Available << " indices/table" << std::endl;

        unsigned minShift = 0;
        unsigned maxShift = 0;
        for(unsigned shift=0;;++shift)
        {
            const size_t n = one << shift;
            const size_t x = CountXBR(n);
            if(x>0&&minShift<=0) { maxShift = minShift = shift; }
            if(x>Available) break;
            maxShift = shift;
            std::cerr << "-- 2^" << std::setw(2) << shift << " = " << std::setw(8) << n << " => " << x << std::endl;
        }

        std::cerr << "-- minShift : 2^" << std::setw(2) << minShift << std::endl;
        std::cerr << "-- maxShift : 2^" << std::setw(2) << maxShift << std::endl;

        const size_t numShift = 1+maxShift - minShift;
        CxxArray<size_t,Memory::Pooled> swaps(numShift);


    }



    FFT:: ~FFT() noexcept
    {

    }
    

}


