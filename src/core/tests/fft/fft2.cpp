
#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/memory/buffer/of.hpp"

using namespace Yttrium;


namespace
{

    template <typename T>
    static inline void CheckFFT2(const unsigned shift)
    {
        const size_t size = 1 << shift;

        Memory::BufferOf< Complex<T> > fft1(size);
        Memory::BufferOf< Complex<T> > fft2(size);
        Memory::BufferOf< T >          data1(size);
        Memory::BufferOf< T >          data2(size);
        Memory::BufferOf< Complex<T> > cplx1(size);

        T *f1 = (&fft1[0].re)-1;
        T *f2 = (&fft2[0].re)-1;
        T *c1 = (&cplx1[0].re)-1;

        T *d1 = &data1[0] - 1;
        T *d2 = &data2[0] - 1;

        for(size_t i=1;i<=size;++i)
        {
            d1[i] = T(i);
            d2[i] = 1+size-i;
            cplx1[i-1] = d1[i];
        }

        Core::Display(std::cerr,&data1[0],size) << std::endl;
        Core::Display(std::cerr,&data2[0],size) << std::endl;
        Core::Display(std::cerr,&cplx1[0],size) << std::endl;

        FFT::Forward(f1, f2, d1, d2, size);
        FFT::Forward(c1,size);
        Core::Display(std::cerr<<"cpx1  = ",c1+1,2*size) << std::endl;

    }

}

Y_UTEST(fft2)
{
    {
        CheckFFT2<float>(3);
    }

}
Y_UDONE()
