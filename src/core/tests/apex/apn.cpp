
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/utest/timing.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/concurrent/singulet.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_n)
{
   // Concurrent::Singulet::Verbose = true;

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
        std::cerr << i << std::endl;
    }
    std::cerr << std::endl;

    for(apn i=begin;i!=end;++i)
    {
       std::cerr << i << std::endl;
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
        std::cerr << i << std::endl;
    }
    std::cerr << std::endl;

    for(apn i=end;i!=begin;i--)
    {
        std::cerr << i << std::endl;
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
                Y_ASSERT( Sign::Of(l,r) == apn::Compare(L,R));
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
    }

    std::cerr << "-- Check Mul64" << std::endl;
    for(unsigned i=0;i<=32;++i)
    {
        for(unsigned j=0;j<=32;++j)
        {
            for(size_t loop=0;loop<16;++loop)
            {
                const uint64_t l = ran.to<uint64_t>(i);
                const uint64_t r = ran.to<uint64_t>(j);
                const uint64_t p = l*r;
                const apn      L = l;
                const apn      R = r;
                const apn      P = L*R;
                Y_ASSERT(p==P);
                {
                    apn dum = L; dum *= R; Y_ASSERT(dum==P);
                }

                {
                    apn dum = L; dum *= r; Y_ASSERT(dum==P);
                }
            }

        }
    }

    std::cerr << "-- Check Mul64" << std::endl;
    for(unsigned i=0;i<=32;++i)
    {
        for(size_t loop=0;loop<64;++loop)
        {
            const uint64_t x = ran.to<uint64_t>(i);
            const uint64_t x2 = x*x;
            const apn      X  = x;
            const apn      X2 = X.sqr();
            const apn      Y2 = X * X;
            Y_ASSERT(X2==x2);
            Y_ASSERT(Y2==X2);
        }
    }

    std::cerr << "-- Check Div64" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        for(unsigned j=1;j<=i;++j)
        {
            for(size_t loop=0;loop<8;++loop)
            {
                const uint64_t numer = ran.to<uint64_t>(i);
                const uint64_t denom = ran.to<uint64_t>(j);
                const uint64_t q     = numer/denom;
                const apn Numer = numer;
                const apn Denom = denom;
                const apn Q     = Numer/Denom;
                //std::cerr << numer << "/" << denom << " = " << Q << "/" << Hexadecimal(q,Hexadecimal::Compact,Hexadecimal::UpperCase) << std::endl;
                Y_ASSERT(Q==q);
            }
        }
    }

    std::cerr << "-- Check Mod64" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        for(unsigned j=1;j<=64;++j)
        {
            for(size_t loop=0;loop<16;++loop)
            {
                const uint64_t numer = ran.to<uint64_t>(i);
                const uint64_t denom = ran.to<uint64_t>(j);
                const uint64_t r     = numer%denom;
                const apn Numer = numer;
                const apn Denom = denom;
                const apn R     = Numer%Denom;
                Y_ASSERT(R==r);
            }
        }
    }

    std::cerr << "-- Check Div, Extended [";
    for(size_t loop=0;loop<128;++loop)
    {
        (std::cerr << '.').flush();
        const apn numer(1+ran.leq(1000),ran);
        const apn denom(1+ran.leq(1000),ran);
        apn q,r;
        apn::Div(q,r,numer,denom);
        Y_ASSERT(q*denom+r==numer);
        if(numer<denom)
        {
            Y_ASSERT(0==q);
        }
        
    }
    std::cerr << "]" << std::endl;


    
    std::cerr << "-- Check Mul/Div [";
    for(size_t loop=0;loop<64;++loop)
    {
        (std::cerr << '.').flush();
        const apn lhs(1+ran.leq(1000),ran);
        const apn rhs(1+ran.leq(1000),ran);
        const apn prod = lhs * rhs;
        const apn ldv = prod/rhs;   Y_ASSERT(ldv==lhs);
        const apn rdv = prod/lhs;   Y_ASSERT(rdv==rhs);
    }
    std::cerr << "]" << std::endl;

    {
        apn a = 15;
        apn b = 5;
        apn g = apn::GCD(a,b);
        std::cerr << "g=" << g << std::endl;
    }

    std::cerr << "-- Check Output" << std::endl;
    for(unsigned i=2;i<=64;++i)
    {

        const uint64_t u = ran.to<uint64_t>(i);
        const apn      n = u;
        std::cerr << std::hex << std::setw(17) << n << " = " << std::dec << std::setw(22) << n << " / " << u << std::endl;
    }



}
Y_UDONE()

