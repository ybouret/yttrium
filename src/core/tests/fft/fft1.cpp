#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include "y/memory/buffer/of.hpp"

using namespace Yttrium;


namespace
{




    template <typename T> static inline
    void checkFFT(const unsigned shift)
    {
        static const  T half(0.5);
        const size_t    size = 1 << shift;
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
        size_t   nok = 0;
        size_t   bad = 0;
        for(size_t i=1;i<=n;++i)
        {
            const size_t ii = static_cast<size_t>(std::floor(s[i]+half));
            if( ii == i ) ++nok; else ++bad;
        }
        std::cerr << " |  +" << std::setw(8) << nok;
        std::cerr << " |  -" << std::setw(8) << bad;
        std::cerr << std::endl;

    }

}

Y_UTEST(fft1)
{

    
    Y_SIZEOF( LongTypeFor<float>::Type );
    Y_SIZEOF( LongTypeFor<double>::Type );
    Y_SIZEOF( LongTypeFor<long double>::Type );

#if 0
    Core::Display(std::cerr << "PosSin=", &FFT::Table<double>::PositiveSin[0], 64) << std::endl;
    Core::Display(std::cerr << "PosSin=", &FFT::Table<long double>::PositiveSin[0], 64) << std::endl;
    Core::Display(std::cerr << "NegSin=", &FFT::Table<double>::NegativeSin[0], 64) << std::endl;
    Core::Display(std::cerr << "NegSin=", &FFT::Table<long double>::NegativeSin[0], 64) << std::endl;
#endif

#if 0
    for(unsigned shift=00; shift<=8; ++shift)
    {
        const size_t                        size = 1 << shift;
        std::cerr << "size=" << size << std::endl;
        Memory::BufferOf< Complex<double> > source(size);
        const size_t n = size << 1;
        double *s = &(source[0].re)-1;

        for(size_t i=1;i<=n;++i) s[i] = double(i);

        FFT::Forward(s,size);
    }

    return 0;
#endif

    for(unsigned shift=00; shift<=20; ++shift)
    {
        checkFFT<float>(shift);
        checkFFT<double>(shift);
        checkFFT<long double>(shift);
    }



}
Y_UDONE()

