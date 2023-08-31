
#include "y/apex/ortho/family.hpp"
#include "y/apex/ortho/space.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/bits.hpp"
#include "y/container/light-array.hpp"
#include "../main.hpp"

using namespace Yttrium;


Y_UTEST(apex_ortho)
{
    Random::Rand ran;

    {
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

    {
        Matrix<apz> P(3,8);
        for(size_t i=1;i<=P.rows;++i)
        {
            for(size_t j=1;j<=P.cols;++j)
            {
                P[i][j] = ran.in<int>(-5,5);
            }
        }
        std::cerr << "P=" << P << std::endl;

        Apex::Ortho::Space::Build(P);
    }




}
Y_UDONE()


