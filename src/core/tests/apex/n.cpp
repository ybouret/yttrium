
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_n)
{
    Random::ParkMiller ran;

    std::cerr << "Set/Assign 64-bits" << std::endl;
    for(unsigned iter=0;iter<1024;++iter)
    {
        const uint64_t u = ran.to<uint64_t>();
        Natural        U = u;
        Y_ASSERT(U.lsw() == u );
        const uint64_t v = ran.to<uint64_t>();
        U  = v;
        Y_ASSERT(U.lsw() == v);
    }

    std::cerr << "Compare 64-bits" << std::endl;
    for(unsigned ubits=0;ubits<=64;++ubits)
    {

        for(size_t uter=0;uter<16;++uter)
        {
            const uint64_t u = ran.to<uint64_t>(ubits);
            const Natural  U = u;
            Y_ASSERT(U._block().bits == ubits);
            Y_ASSERT(U.lsw() == u );

            Y_ASSERT( U == u );
            Y_ASSERT( u == U );
            Y_ASSERT( ! (U!=u) );
            Y_ASSERT( ! (u!=U) );


            for(unsigned vbits=0;vbits<=64;++vbits)
            {

                for(size_t vter=0;vter<16;++vter)
                {
                    const uint64_t v = ran.to<uint64_t>(vbits);
                    const Natural  V = v;

                    switch( Sign::Of(u,v) )
                    {
                        case Negative:
                            Y_ASSERT(Negative == Natural::Compare(U,V));
                            Y_ASSERT(Negative == Natural::Compare(U,v));
                            break;

                        case __Zero__:
                            Y_ASSERT(__Zero__ == Natural::Compare(U,V));
                            Y_ASSERT(__Zero__ == Natural::Compare(U,v));
                            break;

                        case Positive:
                            Y_ASSERT(Positive == Natural::Compare(U,V));
                            Y_ASSERT(Positive == Natural::Compare(U,v));
                            break;
                    }
                }

            }

        }
    }

    return 0;

    for(unsigned lbits=0;lbits<=1024;lbits += 4)
    {
        for(size_t lcycle=0;lcycle<16;++lcycle)
        {
            const Natural lhs(ran,lbits);

            for(unsigned rbits=0;rbits<=1024; rbits += 4)
            {
                for(size_t rcycle=0;rcycle<16;++rcycle)
                {
                    const Natural rhs(ran,lbits);

                }

            }
        }

    }



}
Y_UDONE()
