
#include "y/fft/fft.hpp"


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

    FFT:: FFT()
    {
        static const size_t one =  1;
        size_t maxShift = 0;
        size_t maxSwaps = 0;
        while(true)
        {
            const size_t nswp = CountXBR(one<<maxShift);
            if(nswp*sizeof(uint16_t)>=4096)
                break;
            ++maxShift;
            maxSwaps = nswp;
        }
        std::cerr << "up to " << maxSwaps << std::endl;
    }



    FFT:: ~FFT() noexcept
    {

    }
    

}


