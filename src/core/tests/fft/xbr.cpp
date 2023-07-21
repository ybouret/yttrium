
#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/complex.hpp"
#include "y/memory/buffer/of.hpp"

using namespace Yttrium;


namespace
{


    template <typename T>
    static inline size_t scalarXBR(T data[], const size_t nn) noexcept
    {
        const size_t n = (nn<<1);
        size_t j=1;
        for(size_t i=1;i<n;i+=2)
        {
            if(j>i)
            {
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
    static inline size_t myXBR(T data[], const size_t nn) noexcept
    {
        const size_t n = (nn<<1);
        size_t j=1;
        for(size_t i=1;i<n;i+=2)
        {
            if(j>i)
            {
                
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

    template <typename T> static inline
    void checkXBR(const size_t shift)
    {
        std::cerr << "Check XBR 2^" << shift << std::endl;
        const size_t nc = 1<<shift;
        const size_t nr = nc*2;
        Memory::BufferOf<T>            rbuf(nr);
        Memory::BufferOf< Complex<T> > cbuf(nr);

        T *r = &rbuf[0];
        T *c = &cbuf[0].re;
        for(size_t i=0;i<nr;++i) r[i] = c[i] = static_cast<T>(i+1);

        Core::Display(std::cerr,r,nr) << std::endl;
        Core::Display(std::cerr,&cbuf[0],nc) << std::endl;

        scalarXBR(r-1,nc);
        Core::Display(std::cerr,r,nr) << std::endl;


    }


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

    checkXBR<float>(0);
    checkXBR<float>(1);
    checkXBR<float>(2);
    checkXBR<float>(3);
    checkXBR<float>(4);

    return 0;

#if 0
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
#endif

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

