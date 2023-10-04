
#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/memory/buffer/of.hpp"
#include "y/container/cxx/array.hpp"

using namespace Yttrium;


namespace
{

    template <typename T>
    static inline void CheckFFT2(const unsigned shift)
    {
        const size_t nr = 1 << shift;
        const size_t nc = nr << 1;

        CxxArray<T> data1(nr);
        CxxArray<T> data2(nr);

        for(size_t i=1;i<=nr;++i)
        {
            data1[i] = T(i);
            data2[i] = T(nr+1-i);
        }

        Core::Display(std::cerr << "data1=", &data1[1], nr) << std::endl;
        Core::Display(std::cerr << "data2=", &data2[1], nr) << std::endl;


        CxxArray<T> fft1a(nc);
        CxxArray<T> fft2a(nc);
        CxxArray<T> fft1b(nc);
        CxxArray<T> fft2b(nc);

        for(size_t i=1;i<=nr;++i)
        {
            const size_t j = 1 + (i-1)*2;
            fft1a[j] = data1[i];
            fft2a[j] = data2[i];
        }

        std::cerr << "original:" << std::endl;
        Core::Display(std::cerr << "fft1a=", &fft1a[1], nc) << std::endl;
        Core::Display(std::cerr << "fft2a=", &fft2a[1], nc) << std::endl;

        FFT::Forward(fft1a.legacy(),nr);
        FFT::Forward(fft2a.legacy(),nr);

        std::cerr << "single x 2:" << std::endl;
        Core::Display(std::cerr << "fft1a=", &fft1a[1], nc) << std::endl;
        Core::Display(std::cerr << "fft2a=", &fft2a[1], nc) << std::endl;

        FFT::Forward(fft1b.legacy(), fft2b.legacy(), data1.legacy(), data2.legacy(), nr);
        std::cerr << "Dual:" << std::endl;
        Core::Display(std::cerr << "fft1b=", &fft1b[1], nc) << std::endl;
        Core::Display(std::cerr << "fft2b=", &fft2b[1], nc) << std::endl;

    }

}

Y_UTEST(fft2)
{
    {
        CheckFFT2<float>(3);
        CheckFFT2<double>(3);
        CheckFFT2<long double>(3);

    }

}
Y_UDONE()
