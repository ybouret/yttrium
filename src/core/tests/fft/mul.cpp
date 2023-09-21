

#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/container/cxx/array.hpp"
#include "y/random/bits.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/type/utils.hpp"

using namespace Yttrium;


static inline size_t fill(Writable<uint8_t> &arr, const uint32_t x)
{
    arr[1] = uint8_t(x);
    arr[2] = uint8_t(x>>8);
    arr[3] = uint8_t(x>>16);
    arr[4] = uint8_t(x>>24);
    for(size_t n=4;n>0;--n)
    {
        if(arr[n]>0) return n;
    }
    return 0;
}

static inline void show(const Readable<uint8_t> &arr)
{
    std::cerr << " =";
    for(size_t i=arr.size();i>0;--i)
    {
        std::cerr << Hexadecimal(arr[i]);
    }
    std::cerr << std::endl;
}


Y_UTEST(fft_mul)
{

    Random::Rand ran;

    CxxArray<uint8_t> u(4);
    CxxArray<uint8_t> v(4);

    const uint32_t U = ran.to<uint32_t>( ran.in<unsigned>(0,32) );
    const size_t   n = fill(u,U);

    std::cerr << "U=" << Hexadecimal(U) << "@" << n << std::endl;
    show(u);

    const uint32_t V = ran.to<uint32_t>( ran.in<unsigned>(0,32) );
    const size_t   m = fill(v,V);

    std::cerr << "V=" << Hexadecimal(V) << "@" << m << std::endl;
    show(v);

    const size_t mn=Max(m,n);
    size_t       nn=1;
    while (nn < mn) nn <<= 1;
    nn <<= 1;
    CxxArray<double> a(nn);
    CxxArray<double> b(nn);
    for(size_t i=1;i<=n;++i) a[i] = u[i];
    for(size_t i=1;i<=m;++i) b[i] = v[i];

    FFT::Forward(&a[1]-1,nn>>1);
    FFT::Forward(&b[1]-1,nn>>1);


}
Y_UDONE()

