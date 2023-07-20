

#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_z)
{
    Random::Rand ran;

    std::cerr << "-- simple setups" << std::endl;
    {
        apz z;
        std::cerr << z << std::endl;
    }

    {
        apz z = 92;
        std::cerr << z << std::endl;
    }

    {
        apz z = -719;
        std::cerr << z << std::endl;
        z =0;
        std::cerr << z << std::endl;
        z = 8001;
        std::cerr << z << std::endl;
        z = -899;
        std::cerr << z << std::endl;
    }

    std::cerr << "-- cmp64" << std::endl;
    {
        for(unsigned ibits=0;ibits<=60;++ibits)
        {

            for(unsigned jbits=0;jbits<=60;++jbits)
            {
                for(size_t loop=0;loop<16;++loop)
                {
                    const int64_t i = int64_t( ran.to<uint64_t>(ibits) ) * ran.intSgn();
                    const int64_t j = int64_t( ran.to<uint64_t>(jbits) ) * ran.intSgn();

                    const apz I = i;
                    const apz J = j;
                    Y_ASSERT(I==I); Y_ASSERT(!(I!=I));
                    Y_ASSERT(I==i); Y_ASSERT(!(I!=i));
                    Y_ASSERT(i==I); Y_ASSERT(!(i!=I));

#define CHECK_OP(OP) do { if(i OP j) { Y_ASSERT(I OP J); Y_ASSERT(i OP J); Y_ASSERT(I OP j); } } while(false)
                    CHECK_OP(==);
                    CHECK_OP(!=);
                    CHECK_OP(<);
                    CHECK_OP(>);
                    CHECK_OP(<=);
                    CHECK_OP(>=);

                    if(i>=0)
                    {
                        const apn U = uint64_t(i);
                        if(i<j)  { Y_ASSERT(U<J);   }
                        if(j<i)  { Y_ASSERT(J<U);   }
                        if(i<=j) { Y_ASSERT(U<=J);  }
                        if(j<=i) { Y_ASSERT(J<=U);  }
                        if(i>j)  { Y_ASSERT(U>J);   }
                        if(j>i)  { Y_ASSERT(J>U);   }
                        if(i>=j) { Y_ASSERT(U>=J);  }
                        if(j>=i) { Y_ASSERT(J>=U);  }
                        if(i==j) { Y_ASSERT(U==J); Y_ASSERT(J==U); }
                        if(i!=j) { Y_ASSERT(U!=J); Y_ASSERT(J!=U); }

                    }
                }
            }
        }
    }

    std::cerr << "-- add64" << std::endl;
    {
        for(unsigned ibits=0;ibits<=60;++ibits)
        {
            for(unsigned jbits=0;jbits<=60;++jbits)
            {
                for(size_t loop=0;loop<16;++loop)
                {
                    const int64_t i = int64_t( ran.to<uint64_t>(ibits) ) * ran.intSgn();
                    const int64_t j = int64_t( ran.to<uint64_t>(jbits) ) * ran.intSgn();
                    const int64_t s = i+j;
                    const apz     I = i;
                    const apz     J = j;
                    const apz     S = I+J;
                    Y_ASSERT(S==s);
                    Y_ASSERT(S==I+j);
                    Y_ASSERT(S==i+J);
                    { apz dum = I; dum += J; Y_ASSERT(dum==S); }
                    { apz dum = I; dum += j; Y_ASSERT(dum==S); }
                    if(i>=0)
                    {
                        const apn U = uint64_t(i);
                        { const apz S2 = U+J; Y_ASSERT(S==S2); }
                        { const apz S2 = J+U; Y_ASSERT(S==S2); }
                        { apz dum = J; dum += U; Y_ASSERT(S==dum); }
                    }
                }
            }
        }
    }

    std::cerr << "-- sub64" << std::endl;
    {
        for(unsigned ibits=0;ibits<=60;++ibits)
        {
            for(unsigned jbits=0;jbits<=60;++jbits)
            {
                for(size_t loop=0;loop<16;++loop)
                {
                    const int64_t i = int64_t( ran.to<uint64_t>(ibits) ) * ran.intSgn();
                    const int64_t j = int64_t( ran.to<uint64_t>(jbits) ) * ran.intSgn();
                    const int64_t s = i-j;
                    const apz     I = i;
                    const apz     J = j;
                    const apz     S = I-J;
                    Y_ASSERT(S==s);
                    Y_ASSERT(S==I-j);
                    Y_ASSERT(S==i-J);
                    { apz dum = I; dum -= J; Y_ASSERT(dum==S); }
                    { apz dum = I; dum -= j; Y_ASSERT(dum==S); }
                    if(i>=0)
                    {
                        const apn U = uint64_t(i);
                        { const apz S2 = U-J;    Y_ASSERT(S==S2); }
                        { const apz S2 = -(J-U); Y_ASSERT(S==S2); }
                        { apz dum = J; dum -= U; dum = -dum; Y_ASSERT(S==dum); }
                    }
                }
            }
        }
    }

    std::cerr << "-- mul64" << std::endl;
    {
        for(unsigned ibits=0;ibits<=30;++ibits)
        {
            for(unsigned jbits=0;jbits<=30;++jbits)
            {
                for(size_t loop=0;loop<16;++loop)
                {
                    const int64_t i = int64_t( ran.to<uint64_t>(ibits) ) * ran.intSgn();
                    const int64_t j = int64_t( ran.to<uint64_t>(jbits) ) * ran.intSgn();
                    const int64_t p = i*j;
                    const apz     I = i;
                    const apz     J = j;
                    const apz     P = I*J;
                    Y_ASSERT(P==p);
                    Y_ASSERT(I*j==P);
                    Y_ASSERT(i*J==P);
                    { apz dum = I; dum *= J; assert(P==dum); }
                    { apz dum = I; dum *= j; assert(P==dum); }
                    if(i>=0)
                    {
                        const apn U = uint64_t(i);
                        assert(J*U==P);
                        assert(U*J==P);
                        { apz dum = J; dum *= U; assert(dum==P); }
                    }
                }
            }
        }
    }

    std::cerr << "-- div64" << std::endl;
    {
        for(unsigned ibits=0;ibits<=30;++ibits)
        {
            for(unsigned jbits=1;jbits<=30;++jbits)
            {
                for(size_t loop=0;loop<16;++loop)
                {
                    const int64_t i = int64_t( ran.to<uint64_t>(ibits) ) * ran.intSgn();
                    const int64_t j = int64_t( ran.to<uint64_t>(jbits) ) * ran.intSgn();
                    const int64_t q = i/j;
                    const apz     I = i;
                    const apz     J = j;
                    const apz     Q = I/J;
                    Y_ASSERT(Q==q);

                    Y_ASSERT(I/j==Q);
                    Y_ASSERT(i/J==Q);
                    { apz dum = I; dum /= J; assert(Q==dum); }
                    { apz dum = I; dum /= j; assert(Q==dum); }
                    if(i>=0)
                    {
                        const apn U = uint64_t(i);
                        assert(U/J == Q);
                    }
                    if(j>0)
                    {
                        const apn V = uint64_t(j);
                        assert(I/V  == Q);
                    }
                }
            }
        }
    }


    std::cerr << "-- add/sub" << std::endl;
    for(size_t i=0;i<1000;++i)
    {
        const apz lhs(ran.leq(1000),ran);
        const apz rhs(ran.leq(1000),ran);
        const apz sum = lhs + rhs;
        { const apz dum = sum-rhs; Y_ASSERT(lhs==dum); }
        { const apz dum = sum-lhs; Y_ASSERT(rhs==dum); }
    }

    std::cerr << "-- mul/div" << std::endl;
    for(size_t i=0;i<100;++i)
    {
        const apz lhs(1+ran.leq(1000),ran);
        const apz rhs(1+ran.leq(1000),ran);
        const apz prod = lhs * rhs;
        { const apz dum = prod/rhs; Y_ASSERT(lhs==dum); }
        { const apz dum = prod/lhs; Y_ASSERT(rhs==dum); }
    }






}
Y_UDONE()
