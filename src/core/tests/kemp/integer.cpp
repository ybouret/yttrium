#include "y/kemp/integer.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_integer)
{
    Random::ParkMiller ran;


    std::cerr << "<Output>" << std::endl;
    for(size_t k=0;k<10;++k)
    {
        const int64_t i = ran.to<int64_t>();
        const apz     I = i;
        std::cerr << std::setw(24) << i << " => " << I << std::endl;
    }

    std::cerr << "<Compare64>" << std::endl;
    for(size_t k=0;k<1000;++k)
    {
        const int64_t  i   = ran.to<int64_t>();
        const int64_t  j   = ran.to<int64_t>();
        const SignType cmp = Sign::Of(i,j);
        const apz      I   = i;
        const apz      J   = j;
        Y_ASSERT( Integer::Compare(I,J) == cmp );
        Y_ASSERT( Integer::Compare(I,j) == cmp );
        Y_ASSERT( Integer::Compare(i,J) == cmp );
        
        if(i>=0)
        {
            const Natural n = i;
            Y_ASSERT( Integer::Compare(n,J) == cmp );
        }

        if(j>=0)
        {
            const Natural n = j;
            Y_ASSERT( Integer::Compare(I,n) == cmp );
        }
    }

    for(size_t k=0;k<10;++k)
    {
        std::cerr << std::setw(5)  << int(ran.to<int8_t>());
        std::cerr << std::setw(8)  << (ran.to<int16_t>());
        std::cerr << std::setw(12) << (ran.to<int32_t>());
        std::cerr << std::setw(22) << (ran.to<int64_t>());
        std::cerr << std::endl;
    }

    std::cerr << "<Add64/Sub64>" << std::endl;
    bool found = false;
    for(size_t k=0; !found || k<100;++k)
    {
        const int64_t  i   = ran.to<int32_t>();
        const int64_t  j   = ran.to<int32_t>();
        const apz      I   = i;
        const apz      J   = j;
        {
            const int64_t sum = i+j;
            const int64_t dif = i-j;

            Y_ASSERT( I+J == sum );
            Y_ASSERT( I+j == sum );
            Y_ASSERT( i+J == sum );
           
            Y_ASSERT( I-J == dif );
            Y_ASSERT( I-j == dif );
            Y_ASSERT( i-J == dif );

            { apz S = I; S += J; Y_ASSERT(S==sum); }
            { apz S = I; S += j; Y_ASSERT(S==sum); }
            { apz D = I; D -= J; Y_ASSERT(D==dif); }
            { apz D = I; D -= j; Y_ASSERT(D==dif); }
            if(j>=0)
            {
                if(!found)
                {
                    found = true;
                    std::cerr << '*';
                }
                const apn N = j;
                Y_ASSERT( I+N == sum );
                Y_ASSERT( N+I == sum );
                Y_ASSERT( I-N == dif );
                Y_ASSERT( N-I == -dif );
                { apz S = I; S += N; Y_ASSERT(S==sum); }
                { apz D = I; D -= N; Y_ASSERT(D==dif); }
            }
        }
    }
    std::cerr << std::endl;

    std::cerr << "<Mul64/Div64>" << std::endl;
    for(unsigned i=0;i<=31;++i)
    {
        for(unsigned j=0;j<=31;++j)
        {
            const int64_t l = ran.choice() ? -int64_t( ran.to<uint32_t>(i) ) : int64_t( ran.to<uint32_t>(i) );
            const int64_t r = ran.choice() ? -int64_t( ran.to<uint32_t>(i) ) : int64_t( ran.to<uint32_t>(i) );
            const int64_t prod = l*r;
            const apz L = l;
            const apz R = r;
            const apz P = L*R;
            Y_ASSERT(P   == prod);
            Y_ASSERT(l*R == prod);
            Y_ASSERT(R*l == prod);
            { apz X = L; X *= R; Y_ASSERT(X==P); };
            { apz X = L; X *= r; Y_ASSERT(X==P); };


            if(r>0)
            {
                Y_ASSERT(L == P/R);
                Y_ASSERT(L == P/r);
                Y_ASSERT(L == prod/R);
            }


            if(r>=0)
            {
                const apn N = r;
                Y_ASSERT(L*N==P);
                Y_ASSERT(N*L==P);
                { apz X = L; X *= N; Y_ASSERT(X==P); };
                if(r>0)
                {
                    apz Q = P; Q /= N; Y_ASSERT(Q==L);
                }
            }
        }
    }

    std::cerr << "<Compact>" << std::endl;
    const int m = 10;
    for(int64_t i=-m;i<=m;++i)
    {
        const apz I = i;
        for(int64_t j=-m;j<=m;++j)
        {
            const apz J = j;
            Y_ASSERT(I+J==i+j);
            Y_ASSERT(I*J==i*j);
            if(j!=0)
            {
                Y_ASSERT(I/J==i/j);
            }
        }
    }

    for(apz i=-m;i<=m;++i) std::cerr << i << "/";
    std::cerr << std::endl;
 
    for(apz i=-m;i<=m;i++) std::cerr << i << "/";
    std::cerr << std::endl;


    for(apz i=m;i>=-m;--i) std::cerr << i << "/";
    std::cerr << std::endl;

    for(apz i=m;i>=-m;i--) std::cerr << i << "/";
    std::cerr << std::endl;

    std::cerr << "<Cast>" << std::endl;
    {
        apz z = 100;
        std::cerr << z << "=>" << int(z.cast<uint8_t>("z")) << std::endl;
        z = -100;
        std::cerr << z << "=>" << int(z.cast<int8_t>("z")) << std::endl;

    }



}
Y_UDONE()
