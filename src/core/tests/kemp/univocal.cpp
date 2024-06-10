
#include "y/kemp/rational/univocal.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_univocal)
{
    Random::ParkMiller ran;

    {
        for(size_t cycle=0;cycle<=10;++cycle)
        {
            for(size_t i=0;i<=6;++i)
            {
                Vector<apq> Q;
                for(size_t j=0;j<i;++j)
                    Q << apq(8,8,ran);
                Vector<apq> U(Q);
                Univocal::Make(U);
                std::cerr << Q << "->" << U << std::endl;
            }
        }
    }

    {
        Matrix<int> a(5,5);
        for(size_t i=1;i<=a.rows;++i)
        {
            for(size_t j=1;j<=a.cols;++j)
            {
                a[i][j] = ran.in<int>(-5,5);
            }
        }
        std::cerr << "a=" << a << std::endl;
        Univocal::MakeMatrixCast(a);
        std::cerr << "u=" << a << std::endl;
    }

}
Y_UDONE()
