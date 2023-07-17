
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/utest/timing.hpp"
#include "y/text/hexadecimal.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_n)
{
    Random::Rand ran;

    std::cerr << "-- Check Init64" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        for(size_t loop=0;loop<4;++loop)
        {
            const uint64_t n = ran.to<uint64_t>(i);
            APN            N = n;
            //N.printHex(std::cerr) << "/" << Hexadecimal(n,Hexadecimal::Compact,Hexadecimal::UpperCase) << std::endl;
            Y_ASSERT( uint64_t(N) == n);
            APN            M = N;
            Y_ASSERT( uint64_t(M) == n);
            const uint64_t m = ran.to<uint64_t>(i);
            M = m;
            Y_ASSERT( uint64_t(M) == m);
            M = N;
            Y_ASSERT( uint64_t(M) == n);
        }
    }
    std::cerr << "-- Check Add64" << std::endl;
    for(unsigned i=0;i<=63;++i)
    {
        for(unsigned j=0;j<=63;++j)
        {
            for(size_t loop=0;loop<16;++loop)
            {
                const uint64_t l = ran.to<uint64_t>(i);
                const uint64_t r = ran.to<uint64_t>(j);
                const uint64_t s = l+r;
                const APN      L = l;
                const APN      R = r;
                const APN      S = L+R;
                Y_ASSERT(uint64_t(S)==s);
                {
                    APN dum = L; dum += R;
                    Y_ASSERT( uint64_t(dum)==s);
                }
                {
                    APN dum = L; dum += r;
                    Y_ASSERT( uint64_t(dum)==s);
                }
            }
        }
    }

    for(APN i=0;i<10;++i)
    {
        i.printHex(std::cerr) << ' ';
    }
    std::cerr << std::endl;

    for(APN i=0;i<10;i++)
    {
        i.printHex(std::cerr) << ' ';
    }
    std::cerr << std::endl;



}
Y_UDONE()

