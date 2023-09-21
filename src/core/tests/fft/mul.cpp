

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

    std::cerr << "U=" << Hexadecimal(U) << "@" << n << " =" << U << std::endl;
    show(u);

    const uint32_t V = ran.to<uint32_t>( ran.in<unsigned>(0,32) );
    const size_t   m = fill(v,V);

    std::cerr << "V=" << Hexadecimal(V) << "@" << m << " = " << V << std::endl;
    show(v);

    const uint64_t W = uint64_t(U) * uint64_t(V);
    std::cerr << "W=" << Hexadecimal(W) << std::endl;

    const size_t nr = NextPowerOfTwo( Max(n,m) )<<1;
    const size_t nc = nr << 1;

    CxxArray<double> a(nc);
    for(size_t i=1;i<=n;++i)
    {
        a[1+((i-1)<<1)] = u[i];
    }
    for(size_t i=1;i<=m;++i)
    {
        a[2+((i-1)<<1)] = v[i];
    }
    Core::Display(std::cerr, &a[1],nc) << std::endl;
    FFT::Forward(&a[1]-1,nr);
    CxxArray<double> b(nc);
    FFT::Expand(&a[1]-1, &b[1]-1,nr);

    //b[1] *= a[1];
    //b[2] *= a[2];
    for(size_t j=1;j<=nc;j+=2)
    {
        const double t = b[j];
        b[j]  = t*a[j]   - b[j+1]*a[j+1];
        b[j+1]= t*a[j+1] + b[j+1]*a[j];
    }
    FFT::Reverse(&b[1]-1,nr);
    Core::Display(std::cerr << "p=", &b[1], nc) << std::endl;
    double cy=0.0;
    for(size_t j=nr;j>0;--j) {
        const size_t k=1+(j-1)*2;
        const double t=b[k]/nr+cy+0.5;
        cy=(unsigned long) (t/256.0);
        b[k]   = t-cy*256.0;
        b[k+1] = 0;
    }
    std::cerr << "cy=" << cy << std::endl;
    Core::Display(std::cerr << "p=", &b[1], nc) << std::endl;

    CxxArray<uint8_t> w(n+m);
    w[1]=(unsigned char) cy;
    for(size_t j=2;j<=n+m;j++)
    {
        const size_t k=1+(j-2)*2;
        std::cerr << "j=" << j << " -> " << k << " / nc=" << nc << std::endl;
        w[j]=(unsigned char) b[k];
    }
    show(w);



}
Y_UDONE()

