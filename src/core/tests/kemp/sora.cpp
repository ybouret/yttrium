
#include "y/kemp/sora.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_sora)
{
    Random::ParkMiller ran;

    std::cerr << "Simplification" << std::endl;
    {
        Vector<apq> Q;
        for(size_t n=0;n<=5;++n)
        {
            Q.free();
            for(size_t i=0;i<n;++i) Q << apq(10,10,ran);
            std::cerr << Q << std::endl;
            const Natural g = Sora::CommonDenominator(Q);
            std::cerr << "g=" << g << std::endl;
            std::cerr << Sora::Simplify(Q) << std::endl;

        }
    }


    std::cerr << "Colinearity" << std::endl;
    {
        Vector<int> V1, V2;
        Y_CHECK( Sora::AreColinear(V1,V2) );
        V1 << 0;
        V2 << 0;
        Y_CHECK( Sora::AreColinear(V1,V2) );
        V1 << 1;
        V2 << 2;
        Y_CHECK( Sora::AreColinear(V1,V2) );
        V1 << -3;
        V2 << 0;
        Y_CHECK( ! Sora::AreColinear(V1,V2) );
        V2.tail() = -6;
        Y_CHECK( Sora::AreColinear(V1,V2) );
    }

    {
        for(size_t cycle=0;cycle<=10;++cycle)
        {
            for(size_t i=0;i<=6;++i)
            {
                Vector<apq> Q;
                for(size_t j=0;j<i;++j)
                    Q << apq(8,8,ran);
                Vector<apq> U(Q);
                Sora::Univocal(U);
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
        Sora::MakeUnivocalCast(a);
        std::cerr << "u=" << a << std::endl;
    }




}
Y_UDONE()
