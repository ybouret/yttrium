
#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/calculus/align.hpp"

using namespace Yttrium;


namespace
{

    static inline size_t CountXBR(const size_t nn, uint64_t &imx) noexcept
    {
        size_t count = 0;
        const size_t n = (nn<<1);
        imx = 0;
        size_t j=1;
        for(size_t i=1;i<n;i+=2)
        {
            if(j>i)
            {
                //Swap(data[j],data[i]);
                //Swap(data[j+1],data[i+1]);
                //std::cerr << i << " <-> " << j << " AND " << (i+1) << " <-> " << (j+1) << std::endl;
                imx = Max<uint64_t>(imx,j+1);
                count += 2;
            }
            size_t m=nn;
            while (m >= 2 && j > m)
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        std::cerr << "number = " << nn << ", #swaps=" << count << ", imx=" << imx << std::endl;
        return count;
    }

    static double Duration = 0.5;

    template <typename T>
    static inline
    uint64_t  testXBR(const size_t n)
    {
        Memory::Legacy ram;
        Timing         tmx;

        const size_t nn  = n*2;
        const size_t bs  = nn*sizeof(T);
        T          *data = static_cast<T *>(ram.acquire(bs)) - 1;

        for(size_t i=1;i<=nn;++i) data[i] = T(i);

        Y_Timing(tmx, FFT::XBR(data,n), Duration);



        ram.release(data+1,bs);
        return tmx.speed();
    }


    
}

Y_UTEST(fft_xbr)
{

    if(true)
    {
        for(size_t i=0;i<=20;++i)
        {
            const size_t n   = (1<<i);
            uint64_t     imx = 0;
            const size_t ns  = CountXBR(n,imx);
            const unsigned maxbits   = BitCount::For(imx);
            const size_t   maxbytes  = Y_ALIGN_ON(8,maxbits)/8;
            const size_t   wordSize  = NextPowerOfTwo(maxbytes);
            const size_t   wordBits  = 8 * wordSize;
            const size_t   tableSize = wordSize * ns;
            std::cerr << "|_nswap=" << ns << ", coded on " << maxbits << " => uint" << wordBits << "_t" <<  " tableSize=" << HumanReadable(tableSize) << std::endl;
        }
    }


    for(size_t i=0;i<=10;++i)
    {
        const size_t n   = (1<<i);
        (std::cerr << "Speed @" << std::setw(8) << n << " : ").flush();
        const uint64_t rateF = testXBR<float>(n);
        const uint64_t rateD = testXBR<double>(n);
        const uint64_t rateL = testXBR<long double>(n);
        std::cerr << HumanReadable(rateF) << " " << HumanReadable(rateD) << " " << HumanReadable(rateL) << std::endl;
    }

}
Y_UDONE()

