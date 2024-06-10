
#include "y/kemp/narrow.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_narrow)
{
    Random::ParkMiller ran;


    Matrix<int> C;
    Matrix<int> M;
    for(size_t nr=1;nr<=10;++nr)
    {
        std::cerr << "#nr=" << nr << std::endl;
        for(size_t nc=1;nc<=4;++nc)
        {
            M.make(nr,nc);
            for(size_t i=1;i<=nr;++i)
                for(size_t j=1;j<=nc;++j)
                {
                    M[i][j] = ran.in<int>(-3,3);
                }
            std::cerr << "M=" << M << std::endl;
            Narrow::Down(C,M);
            std::cerr << "C=" << C << std::endl;
        }
    }

}
Y_UDONE()
