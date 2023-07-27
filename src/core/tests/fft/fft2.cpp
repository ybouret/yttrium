
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
        Memory::BufferOf<T>            data1(size);
        Memory::BufferOf<T>            data2(size);
        Memory::BufferOf< Complex<T> > fft1(size);
        Memory::BufferOf< Complex<T> > fft2(size);
        Memory::BufferOf< Complex<T> > test(size);

        Y_ASSERT(data1.measure() == data2.measure());
        Y_ASSERT(fft1.measure() == fft2.measure());
        Y_ASSERT(fft1.measure() == 2 * data1.measure());

        for(size_t i=0;i<size;++i)
        {
            data1[i] = i+1;
            test[i]  = i+1;
            data2[i] = size-i;
        }

        T *r1 = (&data1[0])   - 1;
        T *r2 = (&data2[0])   - 1;


        T *c1 = (&fft1[0].re) - 1;
        T *c2 = (&fft2[0].re) - 1;

        Core::Display(std::cerr, &data1[0],size) << std::endl;
        Core::Display(std::cerr, &test[0], size) << std::endl;
        Core::Display(std::cerr, &data2[0],size) << std::endl;


        FFT::Forward(c1,
                     c2,
                     r1,
                     r2,
                     size);

        FFT::Forward(&(test[0].re) -1, size);

        Core::Display(std::cerr << "fft1=", &fft1[0],size) << std::endl;
        Core::Display(std::cerr << "test=", &test[0],size) << std::endl;
        Core::Display(std::cerr << "fft2=", &fft2[0],size) << std::endl;

        FFT::Reverse(c1,size);
        for(size_t i=1;i<=size*2;++i)
        {
            c1[i]/=size;
        }
        Core::Display(std::cerr,&fft1[0],size) << std::endl;
    }

}

Y_UTEST(fft2)
{
    {
        CheckFFT2<float>(3);
    }

}
Y_UDONE()
