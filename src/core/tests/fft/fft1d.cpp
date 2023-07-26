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
        std::cerr << "FFT-" << std::setw(3) << sizeof(T)*8 << " : 2^" << std::setw(2) << shift << " : ";

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

        for(size_t i=0;i<size*2;++i)
        {
            Y_ASSERT(i == static_cast<size_t>(std::floor( (r[i]/size) + T(0.5) )));
        }

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

    for(unsigned shift=1; shift<=10; ++shift)
    {
        checkFFT<float>(fft,shift);
        checkFFT<double>(fft,shift);
        checkFFT<long double>(fft,shift);
    }



}
Y_UDONE()

