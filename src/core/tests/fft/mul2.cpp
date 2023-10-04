
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
    std::cerr << " = 0x";
    for(size_t i=arr.size();i>0;--i)
    {
        std::cerr << Hexadecimal(arr[i]);
    }
    std::cerr << std::endl;
}


Y_UTEST(fft_mul2)
{
    Random::Rand ran;
    
    CxxArray<uint8_t> u(4);
    CxxArray<uint8_t> v(4);

    {
        const uint32_t U = ran.to<uint32_t>( ran.in<unsigned>(0,32) );
        const size_t   n = fill(u,U);

        std::cerr << "U= 0x" << Hexadecimal(U) << "@" << n << " = " << U << std::endl;
        show(u);

        const uint32_t V = ran.to<uint32_t>( ran.in<unsigned>(0,32) );
        const size_t   m = fill(v,V);

        std::cerr << "V= 0x" << Hexadecimal(V) << "@" << m << " = " << V << std::endl;
        show(v);

        const uint64_t W = uint64_t(U) * uint64_t(V);
        std::cerr << "W= 0x" << Hexadecimal(W) << " = " << W << std::endl;

        size_t          nn = 1;
        const size_t    mn = Max(n,m);
        while (nn < mn) nn <<= 1;
        nn <<= 1;
        std::cerr << "#nn=" << nn << std::endl;
        



    }

}
Y_UDONE()
