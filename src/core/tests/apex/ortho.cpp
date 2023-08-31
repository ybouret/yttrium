
#include "y/apex/ortho/family.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/bits.hpp"
#include "y/container/light-array.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_ortho)
{
    Random::Rand ran;

    Apex::Ortho::Family F(3);

    while(F.size<F.dimension)
    {
        int arr[3] = {0,0,0};
        for(size_t i=0;i<3;++i) arr[i] = ran.in<int>(-5,5);
        const LightArray<int> V(arr,3);
        if( F.wouldAccept(V) )
        {
            const Apex::Ortho::Vector &B = F.expand();
            std::cerr << "V=" << V << std::endl;
            std::cerr << "B=" << B << std::endl;
        }
    }
    std::cerr << "F=" << F << std::endl;




}
Y_UDONE()


