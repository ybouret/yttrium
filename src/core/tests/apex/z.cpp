

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

    for(size_t iter=0;iter<10;++iter)
    {
        integer_t       z = ran.to<uint64_t>( unsigned(ran.lt(64)) ); if(ran.choice()) z=-z;
        const Integer   Z = z;
        std::cerr << Z << " / " << z << std::endl;
    }



}
Y_UDONE()
