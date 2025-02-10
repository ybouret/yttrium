#include "y/apex/api/univocal.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;


Y_UTEST(apex_uni)
{
    Random::ParkMiller ran;

    {
        for(size_t dim=0;dim<=8;++dim)
        {
            Vector<unsigned> v(dim,0);
            for(size_t cycle=0;cycle<=10;++cycle)
            {
                for(size_t i=1;i<=dim;++i)
                {
                    v[i] = 0;
                    if(ran.choice()) v[i] = ran.in<unsigned>(1,10);
                }

                (std::cerr << "v=" << v).flush();
                const Natural nrm2 = Univocal::MakeUnsigned(v);
                std::cerr << " => " << v << " @" << nrm2 <<std::endl;
            }
        }
    }


    {

        for(size_t dim=0;dim<=8;++dim)
        {
            Vector<int32_t> v(dim,0);
            for(size_t cycle=0;cycle<=10;++cycle)
            {
                for(size_t i=1;i<=dim;++i)
                {
                    v[i] = 0;
                    if(ran.choice()) v[i] = ran.in<int32_t>(-10,10);
                }

                (std::cerr << "v=" << v).flush();
                const Natural nrm2 = Univocal::MakeSigned(v);
                //std::cerr << " => " << v << " @" << nrm2 <<std::endl;
            }
        }
    }
}
Y_UDONE()




