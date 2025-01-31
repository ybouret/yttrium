
#include "y/apex/rational.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_q)
{
    Random::ParkMiller ran;

    std::cerr << Rational(12,3) << std::endl;

}
Y_UDONE()
