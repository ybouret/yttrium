

#include "y/orthogonal/family.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/park-miller.hpp"
#include "y/container/light-array.hpp"

using namespace Yttrium;


Y_UTEST(kemp_ortho)
{
    Random::ParkMiller ran;

    {
        Orthogonal::Family F(3);

        while(F.size<F.dimensions)
        {
            int arr[3] = {0,0,0};
            for(size_t i=0;i<3;++i) arr[i] = ran.in<int>(-5,5);
            const LightArray<int> V(arr,3);
            if( F.wouldAccept(V) )
            {
                const Orthogonal::Vector &B = F.expand();
                std::cerr << "V=" << V << std::endl;
                std::cerr << "B=" << B << std::endl;
            }
        }
        std::cerr << "F=" << F << std::endl;

    }

    Y_SIZEOF(Orthogonal::Vector);
    Y_SIZEOF(apn);
    Y_SIZEOF(apz);
    Y_SIZEOF(apq);





}
Y_UDONE()


