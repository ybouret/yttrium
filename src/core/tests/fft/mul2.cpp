
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

typedef Complex<double> cplx;

static Hexadecimal D2H(const cplx &z)
{
    return Hexadecimal( unsigned(z.re), Hexadecimal::Compact);
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

        size_t          nr = 1;
        const size_t    mn = Max(n,m);
        while (nr < mn) nr <<= 1;
        nr <<= 1;
        std::cerr << "nr=" << nr << std::endl;
        CxxArray<cplx> fft1(nr);
        CxxArray<cplx> fft2(nr);
        for(size_t i=1;i<=n;++i)
        {
            fft1[i].re = u[n+1-i];
        }

        for(size_t i=1; i<=m;++i)
        {
            fft2[i].re = v[m+1-i];
        }


        Core::Display(std::cerr << "fft1 = ", &fft1[1],nr, D2H) << std::endl;
        Core::Display(std::cerr << "fft2 = ", &fft2[1],nr, D2H) << std::endl;

        FFT::Forward(&(fft1[1].re)-1,nr);
        FFT::Forward(&(fft2[1].re)-1,nr);

        for(size_t i=nr;i>0;--i)
        {
            fft1[i] *= fft2[i];
        }

        FFT::Reverse(&(fft1[1].re)-1,nr);


        static const double RX = 256.0;

        double cy=0.0;
        for(size_t j=nr;j>=1;j--)
        {
            const double t =  floor(fft1[j].re/nr+cy+0.5);
            cy=(unsigned long) (t/RX);
            fft1[j].re=uint8_t(t-cy*RX);
        }
        
        CxxArray<uint8_t> w(m+n);
        if(m+n>0)
        {
            w[1]=(unsigned char) cy;
            for (size_t j=2;j<=n+m;j++)
                w[j]=(unsigned char) (fft1[j-1].re);
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
