
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_n)
{
    Random::ParkMiller ran;
    for(unsigned ubits=0;ubits<=64;++ubits)
    {

        for(size_t uter=0;uter<1024;++uter)
        {
            const uint64_t u = ran.to<uint64_t>(ubits);
            const Natural  U = u;
            Y_ASSERT(U._block().bits == ubits);
            Y_ASSERT(U.lsw() == u );

            Y_ASSERT( U == u );
            Y_ASSERT( u == U );
            Y_ASSERT( ! (U!=u) );
            Y_ASSERT( ! (u!=U) );

        }
    }




}
Y_UDONE()
