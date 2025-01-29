

#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_z)
{
    Random::ParkMiller ran;

    std::cerr << "Assign/Comparison" << std::endl;

    for(size_t iter=0;iter<1000;++iter)
    {
        integer_t       z = ran.to<uint64_t>( unsigned(ran.lt(64)) ); if(ran.choice()) z=-z;
        const Integer   Z = z;
        //std::cerr << Z << " / " << z << std::endl;

        Y_ASSERT( Z == z  );
        Y_ASSERT( z == Z  );
        Y_ASSERT( !(z!=Z) );
        Y_ASSERT( !(Z!=z) );
        Y_ASSERT( __Zero__ == Integer::Compare(Z,Z) );
        Y_ASSERT( __Zero__ == Integer::Compare(Z,z) );
        Y_ASSERT( __Zero__ == Integer::Compare(z,Z) );


        integer_t      w = ran.to<uint64_t>( unsigned(ran.lt(64)) ); if(ran.choice()) w=-w;
        const Integer  W = w;
        const SignType s = Sign::Of(z,w);
        Y_ASSERT( s == Integer::Compare(Z,W) );
        Y_ASSERT( s == Integer::Compare(z,W) );
        Y_ASSERT( s == Integer::Compare(Z,w) );


    }



}
Y_UDONE()
