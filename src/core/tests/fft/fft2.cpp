
#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/type/utils.hpp"
#include "y/memory/buffer/of.hpp"
#include "y/container/cxx/array.hpp"
#include <cmath>

using namespace Yttrium;


namespace
{

    template <typename T>
    static inline void CheckFFT2(const unsigned shift)
    {
        std::cerr << "CheckFFT2 2^" << shift << std::endl;

        const size_t nr = 1 << shift;
        const size_t nc = nr << 1;

        CxxArray<T> data1(nr);
        CxxArray<T> data2(nr);

        for(size_t i=1;i<=nr;++i)
        {
            data1[i] = T(i);
            data2[i] = T(nr+1-i);
        }

        //Core::Display(std::cerr << "data1=", &data1[1], nr) << std::endl;
        //Core::Display(std::cerr << "data2=", &data2[1], nr) << std::endl;


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

       // std::cerr << "original:" << std::endl;
       // Core::Display(std::cerr << "fft1a=", &fft1a[1], nc) << std::endl;
       // Core::Display(std::cerr << "fft2a=", &fft2a[1], nc) << std::endl;

        FFT::Forward(fft1a.legacy(),nr);
        FFT::Forward(fft2a.legacy(),nr);

        //std::cerr << "single x 2:" << std::endl;
        //Core::Display(std::cerr << "fft1a=", &fft1a[1], nc) << std::endl;
        //Core::Display(std::cerr << "fft2a=", &fft2a[1], nc) << std::endl;

        FFT::Forward(fft1b.legacy(), fft2b.legacy(), data1.legacy(), data2.legacy(), nr);
        //std::cerr << "Dual:" << std::endl;
        //Core::Display(std::cerr << "fft1b=", &fft1b[1], nc) << std::endl;
        //Core::Display(std::cerr << "fft2b=", &fft2b[1], nc) << std::endl;

        T rms1 = 0;
        T rms2 = 0;
        for(size_t i=nc;i>0;--i)
        {
            rms1 += Squared(fft1a[i]-fft1b[i]);
            rms2 += Squared(fft2a[i]-fft2b[i]);
        }
        rms1 = std::sqrt(rms1/nc);
        rms2 = std::sqrt(rms2/nc);
        std::cerr << " (*) rms1=" << rms1 << std::endl;
        std::cerr << " (*) rms2=" << rms2 << std::endl;

    }

}

Y_UTEST(fft2)
{
    for(unsigned shift=0;shift<=19;++shift)
    {
        CheckFFT2<float>(shift);
        CheckFFT2<double>(shift);
        CheckFFT2<long double>(shift);
    }

}
Y_UDONE()
