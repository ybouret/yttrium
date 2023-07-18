
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
            apn            N = n;
            //N.printHex(std::cerr) << "/" << Hexadecimal(n,Hexadecimal::Compact,Hexadecimal::UpperCase) << std::endl;
            Y_ASSERT(  N.u64() == n);
            apn            M = N;
            Y_ASSERT(  M.u64() == n);
            const uint64_t m = ran.to<uint64_t>(i);
            M = m;
            Y_ASSERT( M.u64() == m);
            M = N;
            Y_ASSERT( M.u64() == n);
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
                const apn      L = l;
                const apn      R = r;
                const apn      S = L+R;
                Y_ASSERT(S.u64()==s);
                {
                    apn dum = L; dum += R;
                    Y_ASSERT(dum.u64()==s);
                }
                {
                    apn dum = L; dum += r;
                    Y_ASSERT(dum.u64()==s);
                }
            }
        }
    }

    const apn begin(71,ran);
    const apn end = begin+10;
    for(apn i=begin;i!=end;++i)
    {
        i.printHex(std::cerr) << std::endl;
    }
    std::cerr << std::endl;

    for(apn i=begin;i!=end;++i)
    {
        i.printHex(std::cerr) << std::endl;
    }
    std::cerr << std::endl;


    std::cerr << "-- Check Sub64" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        for(unsigned j=0;j<=64;++j)
        {
            for(size_t loop=0;loop<16;++loop)
            {
                uint64_t       l = ran.to<uint64_t>(i);
                uint64_t       r = ran.to<uint64_t>(j);
                if(r>l) Swap(l,r);
                const uint64_t d = l-r;
                const apn      L = l;
                const apn      R = r;
                const apn      D = L-R;
                Y_ASSERT(D.u64()==d);
                {
                    apn dum = L; dum -= R;
                    Y_ASSERT( dum.u64() == d);
                }
                {
                    apn dum = L; dum -= r;
                    Y_ASSERT(dum.u64() == d);
                }
            }
        }
    }

    for(apn i=end;i!=begin;--i)
    {
        i.printHex(std::cerr << "0x") << std::endl;
    }
    std::cerr << std::endl;

    for(apn i=end;i!=begin;i--)
    {
        i.printHex(std::cerr << "0x") << std::endl;
    }
    std::cerr << std::endl;

    std::cerr << "-- Check Comparison 64" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        for(unsigned j=0;j<=64;++j)
        {
            for(size_t loop=0;loop<16;++loop)
            {
                const uint64_t l = ran.to<uint64_t>(i);
                const uint64_t r = ran.to<uint64_t>(j);
                const apn      L = l;
                const apn      R = r;
                Y_ASSERT( SignOf(l,r) == apn::Compare(L,R));
                if(l==r) { Y_ASSERT(L==R); Y_ASSERT(l==R); Y_ASSERT(L==r); }
                if(l!=r) { Y_ASSERT(L!=R); Y_ASSERT(l!=R); Y_ASSERT(L!=r); }
                if(l<r)  { Y_ASSERT(L<R);  Y_ASSERT(l<R);  Y_ASSERT(L<r);  }
                if(l<=r) { Y_ASSERT(L<=R); Y_ASSERT(l<=R); Y_ASSERT(L<=r);  }
                if(l>r)  { Y_ASSERT(L>R);  Y_ASSERT(l>R);  Y_ASSERT(L>r);  }
                if(l>=r) { Y_ASSERT(L>=R); Y_ASSERT(l>=R); Y_ASSERT(L>=r);  }
            }
        }
    }

    std::cerr << "-- Bitwise Ops" << std::endl;
    for(unsigned i=0;i<64;++i)
    {
        const uint64_t u = uint64_t(1) << i;
        const apn      p = apn(TwoToThe,i);
        Y_ASSERT(p==u);
        std::cerr << p << std::endl;
    }

}
Y_UDONE()

