

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

    std::cerr << "-- compare64" << std::endl;
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





}
Y_UDONE()
