#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/memory/legacy.hpp"

using namespace Yttrium;


namespace
{

    template <typename T> static inline
    void testFFT(const unsigned shift)
    {
        Memory::Legacy ram;
        std::cerr << "FFT: 2^" << shift << std::endl;
        const size_t n   = 1 << shift;
        const size_t nn  = n*2;
        const size_t bs  = nn*sizeof(T);
        T          *data = static_cast<T *>(ram.acquire(bs)) - 1;

        for(size_t i=1;i<=nn;++i) data[i] = T(i);
        Core::Display(std::cerr,data+1,nn) << std::endl;
        FFT::Run(data,n, 1);
        FFT::Run(data,n,-1);
        for(size_t i=2*n;i>0;--i) data[i] /= n;
        Core::Display(std::cerr,data+1,nn) << std::endl;

        ram.release(data+1,bs);
    }
}

Y_UTEST(fft_1d)
{

    float arr[] = { 1,2, 3,4, 5,6, 7,8, 9,10, 11,12, 13,14, 15,16 };

    Core::Display(std::cerr,arr,sizeof(arr)/sizeof(arr[0])) << std::endl;

    FFT::Run(arr-1,8,1);
    FFT::Run(arr-1,8,-1);

    for(size_t i=0;i<16;++i)
    {
        arr[i] /= 8;
    }
    Core::Display(std::cerr,arr,sizeof(arr)/sizeof(arr[0])) << std::endl;

    testFFT<float>(0);
    testFFT<float>(1);
    testFFT<float>(2);
    testFFT<float>(3);
    testFFT<float>(4);
    testFFT<float>(5);


}
Y_UDONE()

