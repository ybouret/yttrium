#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/memory/buffer/of.hpp"

using namespace Yttrium;


namespace
{



    static double Duration = 0.1;





    template <typename T> static inline
    void checkFFT( FFT &fft, const unsigned shift )
    {

        const size_t size = 1 << shift;
        std::cerr << "FFT-" << std::setw(3) << sizeof(T)*8 << " : 2^" << std::setw(2) << shift << " = " << std::setw(8) << size << " : ";
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

        if(false)
        {
            std::cerr << std::endl;
            fft.forward(&cbuf[0],size,shift);
            fft.reverse(&cbuf[0],size,shift);
            return;
        }
        

        FFT::Forward(r-1,size);
        fft.forward(&cbuf[0],size,shift);
        Y_ASSERT(rbuf.HasSameContentThan(cbuf));
        FFT::Reverse(r-1,size);
        fft.reverse(&cbuf[0],size,shift);
        Y_ASSERT(rbuf.HasSameContentThan(cbuf));

        if(sizeof(T)>=sizeof(double))
        {
            for(size_t i=0;i<size*2;++i)
            {
                const T      real = (std::floor( (r[i]/size) + T(0.5) ));
                const size_t indx = static_cast<size_t>(real);
                if(indx != i)
                {
                    std::cerr << "expected " << i << ", got " << indx << std::endl;
                }
                Y_ASSERT(indx==i);
            }
        }

        Timing tmx;
        tmx.reset();
        while(true)
        {
            for(size_t i=0;i<size*2;++i)
            {
                r[i] = T(i);
            }
            const uint64_t mark = WallTime::Ticks();
            FFT::Forward(r-1,size);
            FFT::Reverse(r-1,size);
            if( tmx.renew(mark).probe() >= Duration)
                break;
        }
        
        std::cerr << " | Run: @" << HumanReadable(tmx.speed());

        tmx.reset();
        while(true)
        {
            for(size_t i=0;i<size*2;++i)
            {
                c[i] = T(i);
            }
            const uint64_t mark = WallTime::Ticks();
            fft.forward(&cbuf[0],size,shift);
            fft.reverse(&cbuf[0],size,shift);
            if( tmx.renew(mark).probe() >= Duration)
                break;
        }

        std::cerr << " | run: @" << HumanReadable(tmx.speed());

        std::cerr << std::endl;

    }

}

Y_UTEST(fft_1d)
{

    if(argc>1) Duration = atof(argv[1]);

    FFT &fft = FFT::Instance();
    Y_SIZEOF( LongTypeFor<float>::Type );
    Y_SIZEOF( LongTypeFor<double>::Type );
    Y_SIZEOF( LongTypeFor<long double>::Type );

    for(unsigned shift=0; shift<=16; ++shift)
    {
        checkFFT<float>(fft,shift);
        checkFFT<double>(fft,shift);
        //checkFFT<long double>(fft,shift);
    }



}
Y_UDONE()

