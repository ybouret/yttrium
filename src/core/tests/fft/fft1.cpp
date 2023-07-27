#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/memory/buffer/of.hpp"

using namespace Yttrium;


namespace
{



    static double Duration = 0.1;

    template <typename T> static inline
    void checkFFT(const unsigned shift)
    {

        const size_t size = 1 << shift;
        std::cerr << "FFT-" << std::setw(3) << sizeof(T)*8 << " : 2^" << std::setw(2) << shift << " = " << std::setw(8) << size << " : ";

        Memory::BufferOf< Complex<T> > source(size);
        const size_t n = size << 1;
        T *s = &(source[0].re)-1;

        for(size_t i=1;i<=n;++i) s[i] = T(i);


        FFT::Forward(s,size);
        FFT::Reverse(s,size);
        for(size_t i=1;i<=n;++i)
        {
            s[i] /= size;
        }
        size_t nok = 0;
        size_t bad = 0;
        for(size_t i=1;i<=n;++i)
        {
            const size_t ii = static_cast<size_t>(std::floor(s[i]+T(0.5)));
            if( ii == i ) ++nok; else ++bad;
        }
        std::cerr << " |  +" << std::setw(8) << nok;
        std::cerr << " |  -" << std::setw(8) << bad;
        std::cerr << std::endl;

    }

}

Y_UTEST(fft1)
{

    if(argc>1) Duration = atof(argv[1]);

    Y_SIZEOF( LongTypeFor<float>::Type );
    Y_SIZEOF( LongTypeFor<double>::Type );
    Y_SIZEOF( LongTypeFor<long double>::Type );

    for(unsigned shift=00; shift<=20; ++shift)
    {
        checkFFT<float>(shift);
        checkFFT<double>(shift);
        checkFFT<long double>(shift);
    }



}
Y_UDONE()

