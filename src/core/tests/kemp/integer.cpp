
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
                Y_ASSERT( I-N == dif );
                { apz S = I; S += N; Y_ASSERT(S==sum); }
                { apz D = I; D -= N; Y_ASSERT(D==dif); }
            }
        }
    }
    std::cerr << std::endl;



}
Y_UDONE()
