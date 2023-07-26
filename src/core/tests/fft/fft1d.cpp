#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/memory/buffer/of.hpp"

using namespace Yttrium;


namespace
{



    static double Duration = 0.5;

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



    template <typename T> static inline
    void checkFFT( FFT &fft, const unsigned shift )
    {
        const size_t size = 1 << shift;
        Memory::BufferOf<T>            rbuf(size*2);
        Memory::BufferOf< Complex<T> > cbuf(size);

        T *r = &rbuf[0];
        T *c = &cbuf[0].re;
        for(size_t i=0;i<size*2;++i)
        {
            r[i] = c[i] = T(i);
        }
        Y_ASSERT(rbuf.HasSameContentThan(cbuf));

        //Core::Display(std::cerr,r,size*2)      << std::endl;
        //Core::Display(std::cerr,&cbuf[0],size) << std::endl;


        FFT::Run(r-1,size,1);
        fft.run(&cbuf[0],size,shift,1);
        Y_ASSERT(rbuf.HasSameContentThan(cbuf));
        FFT::Run(r-1,size,-1);
        fft.run(&cbuf[0],size,shift,-1);
        Y_ASSERT(rbuf.HasSameContentThan(cbuf));

        Timing tmx;
        Y_Timing(tmx,
                 FFT::Run(r-1,size,1);
                 FFT::Run(r-1,size,-1),
                 Duration);
        std::cerr << " | Run: @" << HumanReadable(tmx.speed());

        Y_Timing(tmx,
                 fft.run(&cbuf[0],size,shift,1);
                 fft.run(&cbuf[0],size,shift,-1),
                 Duration);
        std::cerr << " | run: @" << HumanReadable(tmx.speed());
    }

}

Y_UTEST(fft_1d)
{
    
    FFT &fft = FFT::Instance();

    for(unsigned shift=1; shift<=10; ++shift)
    {
        std::cerr << "FFT: 2^" << std::setw(2) << shift << " : ";
        checkFFT<float>(fft,shift);
        std::cerr << std::endl;
    }

    return 0;

    uint64_t rateF[32], rateD[32], rateL[32];
    for(unsigned shift=0;shift<=16;++shift)
    {
        std::cerr << "FFT: 2^" << std::setw(2) << shift << " : ";
        testFFT<float>(shift,rateF[shift]);
        testFFT<double>(shift,rateD[shift]);
        testFFT<long double>(shift,rateL[shift]);
        std::cerr << " | float @"       << HumanReadable(rateF[shift]) << "Op/s";
        std::cerr << " | double @"      << HumanReadable(rateD[shift]) << "Op/s";
        std::cerr << " | long double @" << HumanReadable(rateL[shift]) << "Op/s";

        std::cerr << std::endl;
    }


}
Y_UDONE()

