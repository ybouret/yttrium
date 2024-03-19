#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include "y/memory/buffer/of.hpp"
#include "y/mkl/complex.hpp"
#include <cstring>

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


    template <typename T> static inline
    void checkDualFFT(const unsigned shift)
    {


        const size_t size = 1 << shift;
        const size_t n    = size << 1;

        std::cerr << "check dual @2^" << shift << " = " << size << std::endl;

        Memory::BufferOf< Complex<T> > data1(size);
        Memory::BufferOf< Complex<T> > data2(size);
        Memory::BufferOf< Complex<T> > data3(size);
		

        T *s1 = Memory::OutOfReach::Self(&(data1[0].re)-1);
        T *s2 = Memory::OutOfReach::Self(&(data2[0].re)-1);
        T *s3 = Memory::OutOfReach::Self(&(data3[0].re)-1);

		for (size_t i = 1; i <= n; ++i)
		{
			s1[i] = T(i);
			s2[i] = T(i);
			s3[i] = T(i);
		}
#if 1
        Y_ASSERT(0==memcmp(data1.ro_addr(),data2.ro_addr(),data1.measure()));
        Y_ASSERT(0==memcmp(data1.ro_addr(),data3.ro_addr(),data1.measure()));
        FFT::Forward(s1,size);
        FFT::Forward(s2,s3,size);
        Y_ASSERT(0==memcmp(data1.ro_addr(),data2.ro_addr(),data1.measure()));
        Y_ASSERT(0==memcmp(data1.ro_addr(),data3.ro_addr(),data1.measure()));
#endif
    }


}

#include "y/text/hexadecimal.hpp"



Y_UTEST(fft1)
{


    Y_SIZEOF( LongTypeFor<float>::Type );
    Y_SIZEOF( LongTypeFor<double>::Type );
    Y_SIZEOF( LongTypeFor<long double>::Type );


#if 0
    Y_CHECK(64==sizeof(ScaleTable)/sizeof(ScaleTable[0]));
    for(size_t i=0;i<64;++i)
    {
        std::cerr << ScaleTable[i] << "/";
    }
    std::cerr << std::endl;

    if(false)
    {
        for(unsigned ln=0;ln<64;++ln)
        {
            static const uint64_t one=1;
            const uint64_t        den=one<<ln;
            std::cerr << "1.0/double(Y_U64(0x" <<  Hexadecimal(den) << ")), ";
            std::cerr << std::endl;
        }
        return 0;
    }
#endif
    

#if 0
    Core::Display(std::cerr << "PosSin=", &FFT::Table<double>::PositiveSin[0], 64) << std::endl;
    Core::Display(std::cerr << "PosSin=", &FFT::Table<long double>::PositiveSin[0], 64) << std::endl;
    Core::Display(std::cerr << "NegSin=", &FFT::Table<double>::NegativeSin[0], 64) << std::endl;
    Core::Display(std::cerr << "NegSin=", &FFT::Table<long double>::NegativeSin[0], 64) << std::endl;
    Core::Display(std::cerr << "AuxTab=", &FFT::Table<double>::Minus2SinSq[0], 64) << std::endl;
    Core::Display(std::cerr << "AuxTab=", &FFT::Table<long double>::Minus2SinSq[0], 64) << std::endl;
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

    std::cerr << "Checking Fwd/Rev" << std::endl;
    for(unsigned shift=00; shift<=20; ++shift)
    {
        checkFFT<float>(shift);
        checkFFT<double>(shift);
        checkFFT<long double>(shift);
    }

    std::cerr << "Checking Fwd/Dual" << std::endl;
    for(unsigned shift=00; shift<=20; ++shift)
    {
        checkDualFFT<float>(shift);
        checkDualFFT<double>(shift);
        checkDualFFT<long double>(shift);
    }



}
Y_UDONE()

