#include "y/kemp/rational/univocal.hpp"
#include "y/kemp/colinearity.hpp"
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

            std::cerr << "Naturals" << std::endl;
            for(size_t i=0;i<=6;++i)
            {
                Vector<apn> N;
                for(size_t j=0;j<i;++j)
                    N << apn(4,ran);
                Vector<apn> U(N);
                Univocal::Make(U);
                std::cerr << N << "->" << U << std::endl;
                Y_ASSERT( Colinearity::Of(N,U) );
            }
            std::cerr << std::endl;

            std::cerr << "Integers" << std::endl;
            for(size_t i=0;i<=6;++i)
            {
                Vector<apz> Z;
                for(size_t j=0;j<i;++j)
                    Z << apz(3,ran);
                Vector<apz> U(Z);
                Univocal::Make(U);
                std::cerr << Z << "->" << U << std::endl;
                Y_ASSERT( Colinearity::Of(Z,U) );
            }
            std::cerr << std::endl;


            std::cerr << "Rationals" << std::endl;
            for(size_t i=0;i<=6;++i)
            {
                Vector<apq> Q;
                for(size_t j=0;j<i;++j)
                    Q << apq(4,4,ran);
                Vector<apq> U(Q);
                Univocal::Make(U);
                std::cerr << Q << "->" << U << std::endl;
                Y_ASSERT( Colinearity::Of(Q,U) );
            }
            std::cerr << std::endl;


            std::cerr << "Unsigned" << std::endl;
            for(size_t i=0;i<=6;++i)
            {
                Vector<unsigned> Q;
                for(size_t j=0;j<i;++j)
                    Q << ran.in<unsigned>(0,10);
                Vector<unsigned> U(Q);
                Univocal::Make(U);
                std::cerr << Q << "->" << U << std::endl;
                Y_ASSERT( Colinearity::Of(Q,U) );
            }
            std::cerr << std::endl;


            std::cerr << "Signed" << std::endl;
            for(size_t i=0;i<=6;++i)
            {
                Vector<int> Q;
                for(size_t j=0;j<i;++j)
                    Q << ran.in<int>(-10,10);
                Vector<int> U(Q);
                Univocal::Make(U);
                std::cerr << Q << "->" << U << std::endl;
                Y_ASSERT( Colinearity::Of(Q,U) );
            }
            std::cerr << std::endl;

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
        //std::cerr << "a=" << a << std::endl;
        //Univocal::MakeMatrixCast(a);
        //std::cerr << "u=" << a << std::endl;
    }

}
Y_UDONE()
