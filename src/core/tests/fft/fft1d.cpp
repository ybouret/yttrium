#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"

using namespace Yttrium;


namespace
{

    static inline size_t CountXBR(const size_t nn, size_t &imx) noexcept
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
                imx = Max(imx,j+1);
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


    static double Duration = 1.0;

    template <typename T> static inline
    void testFFT(const unsigned shift, uint64_t &rate)
    {
        Memory::Legacy ram;
        Timing         tmx;
        rate = 0;

        std::cerr << "FFT: 2^" << shift << std::endl;
        const size_t n   = 1 << shift;
        const size_t nn  = n*2;
        const size_t bs  = nn*sizeof(T);
        T          *data = static_cast<T *>(ram.acquire(bs)) - 1;

        for(size_t i=1;i<=nn;++i) data[i] = T(i);
        Core::Display(std::cerr,data+1,nn) << std::endl;
        do
        {
            const uint64_t mark = WallTime::Ticks();
            FFT::Run(data,n, 1);
            FFT::Run(data,n,-1);
            tmx.ticks += WallTime::Ticks() - mark;
            ++tmx.cycle;
            for(size_t i=2*n;i>0;--i) data[i] /= n;
        } while( tmx.probe() < Duration);

        Core::Display(std::cerr,data+1,nn) << std::endl;
        std::cerr << "Speed: " << HumanReadable(tmx.speed()) << std::endl;

        ram.release(data+1,bs);
    }
}

Y_UTEST(fft_1d)
{

    for(size_t i=0;i<10;++i)
    {
        const size_t n   = (1<<i);
        size_t       imx = 0;
        CountXBR(n,imx);
    }

    uint64_t rate[6];
    for(unsigned i=0;i<6;++i)
    {
        testFFT<float>(i,rate[i]);
    }


}
Y_UDONE()

