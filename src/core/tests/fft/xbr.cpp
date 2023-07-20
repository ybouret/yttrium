
#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"

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
                imx = Max<uint64_t>(imx,j);
                ++count;
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

    static double Duration = 1.0;





    template <typename T>
    static inline size_t myXBR(T data[], const size_t nn) noexcept
    {
        const size_t n = (nn<<1);
        size_t j=1;
        for(size_t i=1;i<n;i+=2)
        {
            if(j>i)
            {
                //Exchange<2*sizeof(T)>(data+i,data+j);

                Swap(data[j],data[i]);
                Swap(data[j+1],data[i+1]);
            }
            size_t m=nn;
            while (m >= 2 && j > m)
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        return n;
    }

    template <typename T>
    static inline
    void  testXBR(const size_t n, uint64_t &fftRate, uint64_t &myRate)
    {
        Memory::Legacy ram;
        Timing         tmx;

        const size_t nn  = n*2;
        const size_t bs  = nn*sizeof(T);
        T          *data = static_cast<T *>(ram.acquire(bs)) - 1;

        for(size_t i=1;i<=nn;++i) data[i] = T(i);

        tmx.reset();
        Y_Timing(tmx, FFT::XBR(data,n), Duration);
        fftRate = tmx.speed();

        tmx.reset();
        Y_Timing(tmx,myXBR(data,n), Duration);
        myRate = tmx.speed();


        ram.release(data+1,bs);
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

    Y_SIZEOF(long double);

    for(size_t i=0;i<=12;++i)
    {
        const size_t n   = (1<<i);

        std::cerr << "n= " << std::setw(6) << n << "|";
        {
            (std::cerr << " @F:").flush();
            uint64_t fftRateF = 0, myRateF = 0;
            testXBR<float>(n,fftRateF,myRateF);
            (std::cerr << " fft = " << HumanReadable(fftRateF) << " | opt = " << HumanReadable(myRateF)).flush();
        }

#if 0
        {
            (std::cerr << " @D:").flush();
            uint64_t fftRateD = 0, myRateD = 0;
            testXBR<double>(n,fftRateD,myRateD);
            (std::cerr << " fft = " << HumanReadable(fftRateD) << " | opt = " << HumanReadable(myRateD)).flush();
        }

        {
            (std::cerr << " @L:").flush();
            uint64_t fftRateL = 0, myRateL = 0;
            testXBR<long double>(n,fftRateL,myRateL);
            (std::cerr << " fft = " << HumanReadable(fftRateL) << " | opt = " << HumanReadable(myRateL)).flush();
        }
#endif

        std::cerr << std::endl;
    }

}
Y_UDONE()

