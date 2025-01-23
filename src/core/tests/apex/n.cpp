
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_n)
{
    Random::ParkMiller ran;
    for(unsigned bits=0;bits<=64;++bits)
    {

        const uint64_t u = ran.to<uint64_t>(bits);
        const Natural  n = u;
        Y_ASSERT(n._block().bits == bits);
        Y_ASSERT(n.lsw() == u );
        
        Y_ASSERT( n == u );
        Y_ASSERT( u == n );
        Y_ASSERT( ! (n!=u) );
        Y_ASSERT( ! (u!=n) );

    }




}
Y_UDONE()
