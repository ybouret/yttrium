#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"

using namespace Yttrium;


namespace
{



    static double Duration = 0.1;

    template <typename T> static inline
    void testFFT(const unsigned shift, uint64_t &rate)
    {
        Memory::Legacy ram;
        Timing         tmx;
        rate = 0;

        const size_t n   = 1 << shift;
        const size_t nn  = n*2;
        const size_t bs  = nn*sizeof(T);
        T          *data = static_cast<T *>(ram.acquire(bs)) - 1;

        for(size_t i=1;i<=nn;++i) data[i] = T(i);
        //Core::Display(std::cerr,data+1,nn) << std::endl;
        do
        {
            const uint64_t mark = WallTime::Ticks();
            FFT::Run(data,n, 1);
            FFT::Run(data,n,-1);
            tmx.ticks += WallTime::Ticks() - mark;
            ++tmx.cycle;
            for(size_t i=2*n;i>0;--i) data[i] /= n;
        } while( tmx.probe() < Duration);

        //Core::Display(std::cerr,data+1,nn) << std::endl;
        //std::cerr << "Speed: " << HumanReadable(tmx.speed()) << std::endl;

        for(size_t i=1;i<=nn;++i)
        {
            const size_t ii = static_cast<size_t>(std::floor(data[i]+T(0.5)));
            Y_ASSERT(ii==i);
        }

        rate = tmx.speed();



        ram.release(data+1,bs);
    }
}

Y_UTEST(fft_1d)
{
    

    uint64_t rateF[32], rateD[32], rateL[32];
    for(unsigned shift=0;shift<=8;++shift)
    {
        std::cerr << "FFT: 2^" << std::setw(2) << shift << " : ";
        testFFT<float>(shift,rateF[shift]);
        testFFT<double>(shift,rateD[shift]);
        testFFT<long double>(shift,rateL[shift]);
        std::cerr << " float: " << HumanReadable(rateF[shift]);
        std::cerr << " double: " << HumanReadable(rateD[shift]);
        std::cerr << " long double: " << HumanReadable(rateL[shift]);

        std::cerr << std::endl;
    }


}
Y_UDONE()

