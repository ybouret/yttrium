

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


Y_UTEST(fft_mul)
{

    Random::Rand ran;

    CxxArray<uint8_t> u(4);
    CxxArray<uint8_t> v(4);

    for(size_t iter=0;iter<=0;++iter)
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

        size_t       nn = 1;
        const size_t mn = Max(n,m);
        while (nn < mn) nn <<= 1;
        nn <<= 1;
        std::cerr << "#nn=" << nn << std::endl;
        CxxArray<double> a(nn), b(nn);
        for(size_t i=n;i>0;--i) a[i] = u[n+1-i];
        for(size_t i=m;i>0;--i) b[i] = v[m+1-i];
        //Core::Display(std::cerr << "a=", &a[1], nn) << std::endl;
        //Core::Display(std::cerr << "b=", &b[1], nn) << std::endl;

        FFT::Real(&a[1]-1,nn,1);
        FFT::Real(&b[1]-1,nn,1);

        b[1] *= a[1];
        b[2] *= a[2];
        for(size_t j=3;j<=nn;j+=2)
        {
            const double t = b[j];
            b[j]=t*a[j]-b[j+1]*a[j+1];
            b[j+1]=t*a[j+1]+b[j+1]*a[j];
        }
        FFT::Real(&b[1]-1,nn,-1);

        static const double RX = 256.0;
        double cy=0.0;
        for(size_t j=nn;j>=1;j--)
        {
            const double t=  (b[j]/(nn>>1)+cy+0.5);
            cy=(unsigned long) (t/RX);
           // std::cerr << "t=" << t << ", cy=" << cy << std::endl;
            b[j]=t-cy*RX;
        }
        //std::cerr << "#cy = 0x" << Hexadecimal(uint8_t(cy)) << std::endl;
        CxxArray<uint8_t> w(m+n);
        if(m+n>0)
        {
            w[1]=(unsigned char) cy;
            for (size_t j=2;j<=n+m;j++)
                w[j]=(unsigned char) b[j-1];
            //show(w);
        }

        uint64_t res = 0;
        for(size_t i=1;i<=m+n;++i)
        {
            res <<= 8;
            res |= w[i];
        }
        std::cerr << "p=" << res << std::endl;
    }
}
Y_UDONE()

