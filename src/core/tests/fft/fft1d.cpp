#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"

using namespace Yttrium;


namespace
{



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
    

    uint64_t rate[6];
    for(unsigned i=0;i<6;++i)
    {
        testFFT<float>(i,rate[i]);
    }


}
Y_UDONE()

