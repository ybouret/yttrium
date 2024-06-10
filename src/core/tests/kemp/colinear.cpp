
#include "y/kemp/ops/colinearity.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_colinear)
{
    Random::ParkMiller ran;

    


    std::cerr << "Colinearity" << std::endl;
    {
        Vector<int> V1, V2;
        Y_CHECK( Colinearity::Of(V1,V2) );
        V1 << 0;
        V2 << 0;
        Y_CHECK( Colinearity::Of(V1,V2) );
        V1 << 1;
        V2 << 2;
        Y_CHECK( Colinearity::Of(V1,V2) );
        V1 << -3;
        V2 << 0;
        Y_CHECK( ! Colinearity::Of(V1,V2) );
        V2.tail() = -6;
        Y_CHECK( Colinearity::Of(V1,V2) );
    }

   



}
Y_UDONE()
