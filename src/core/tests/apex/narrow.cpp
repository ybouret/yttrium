

#include "y/apex/api/narrow.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;


Y_UTEST(apex_narrow)
{
    Random::ParkMiller ran;
    Narrow             narrow;

    Matrix<Natural> nmat(4,3);

    Matrix<Integer> target;
    Matrix<int>     source;
    for(size_t r=1;r<=10;++r)
    {
        for(size_t c=1;c<=4;++c)
        {
            source.make(r,c);
            for(size_t i=1;i<=source.rows;++i)
            {
                for(size_t j=1;j<=source.cols;++j)
                {
                    source[i][j] = ran.in<int>(-2,2);
                }
            }
            std::cerr << "source  = " << source << std::endl;
            narrow.down(target,source);
            std::cerr << "target  = " << target << std::endl;
            std::cerr << "nonZero = " << narrow << std::endl;
        }
    }

}
Y_UDONE()
