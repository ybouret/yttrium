
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/type/utils.hpp"
#include "y/container/cxx/array.hpp"
#include "y/fft/fft.hpp"
#include "y/text/hexadecimal.hpp"
#include <cmath>

using namespace Yttrium;


Y_UTEST(fft_apn)
{

    apn u = 0x1234;
    apn v = 0x6;


    const size_t m = u.bytes();
    const size_t n = v.bytes();
   
    size_t       nn = 1;
    const size_t mn = Max(n,m);
    while (nn < mn) nn <<= 1;
    nn <<= 1;
    
    std::cerr << "u   = " << u << " #m=" << m << std::endl;
    std::cerr << "v   = " << v << " #n=" << n << std::endl;
    std::cerr << "#nn = " << nn << std::endl;

    CxxArray<double> a(nn), b(nn);
    for(size_t i=m;i>0;--i) a[i] = u[m-i];
    for(size_t i=n;i>0;--i) b[i] = v[n-i];
    std::cerr << a << std::endl;
    std::cerr << b << std::endl;

    FFT::ForwardReal(&a[1]-1,&b[1]-1,nn);


    b[1] *= a[1];
    b[2] *= a[2];
    for(size_t j=3;j<=nn;j+=2)
    {
        const double t = b[j];
        b[j]=t*a[j]-b[j+1]*a[j+1];
        b[j+1]=t*a[j+1]+b[j+1]*a[j];
    }
    FFT::ReverseReal(&b[1]-1,nn);
    std::cerr << b << std::endl;

    static const double RX = 256.0;
    const size_t        nh = nn>>1;

    double cy=0.0;
    for(size_t j=nn;j>=1;j--)
    {
        const double t =  floor(b[j]/nh+cy+0.5);
        cy=(unsigned long) (t/RX);
        std::cerr << "t=" << t << ", cy=" << cy << std::endl;
        b[j]=uint8_t(t-cy*RX);
    }

    std::cerr << b << std::endl;
    CxxArray<uint8_t> w(m+n);
    if(m+n>0)
    {
        w[1]=(unsigned char) cy;
        for (size_t j=2;j<=n+m;j++)
            w[j]=(unsigned char) b[j-1];
    }

    for(size_t i=1;i<=m+n;++i)
    {
        std::cerr << Hexadecimal(w[i]) << " ";
    }
    std::cerr << std::endl;


}
Y_UDONE()

