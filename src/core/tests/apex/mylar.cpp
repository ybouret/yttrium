#include "y/apex/mylar.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/bits.hpp"
#include "../main.hpp"


using namespace Yttrium;


Y_UTEST(apex_mylar)
{
    Random::Rand ran;

    Vector<apq> Q;
    std::cerr << "ComDen(" << Q << ")=" << Apex::Mylar::CommonDenominator(Q) << std::endl;

    Q << apq(1,2);
    std::cerr << "ComDen(" << Q << ")=" << Apex::Mylar::CommonDenominator(Q) << std::endl;
    Q << apq(1,3);
    std::cerr << "ComDen(" << Q << ")=" << Apex::Mylar::CommonDenominator(Q) << std::endl;

    Q << apq(1,7);
    std::cerr << "ComDen(" << Q << ")=" << Apex::Mylar::CommonDenominator(Q) << std::endl;

    Apex::Mylar::Simplify(Q);
    std::cerr << "Q=" << Q << std::endl;

    Q.free();
    Q << 0 << -1;
    std::cerr << Q << " -> ";
    Apex::Mylar::Univocal(Q);
    std::cerr << Q << std::endl;

    Q.free();
    for(size_t i=4;i>0;--i)
    {
        Q << apq(1+ran.leq(10),1+ran.leq(10),ran);
    }
    std::cerr << Q << std::endl;
    Apex::Mylar::Univocal(Q);
    std::cerr << Q << std::endl;

    {
        Vector<int> lhs;
        Vector<int> rhs;
        std::cerr << Apex::Mylar::AreColinear(lhs,rhs) << std::endl;
        lhs << 1;
        rhs << 2;
        std::cerr << Apex::Mylar::AreColinear(lhs,rhs) << std::endl;

        lhs << 0;
        rhs << 1;
        std::cerr << Apex::Mylar::AreColinear(lhs,rhs) << std::endl;
        rhs.tail() = 0;
        std::cerr << Apex::Mylar::AreColinear(lhs,rhs) << std::endl;

        lhs << 2;
        rhs << 4;
        std::cerr << Apex::Mylar::AreColinear(lhs,rhs) << std::endl;
        lhs << -5;
        rhs << -10;
        std::cerr << Apex::Mylar::AreColinear(lhs,rhs) << std::endl;

    }

    {
        Matrix<uint16_t> source(3,5);
        Matrix<apz>      target;
        FillWritable(source[1],ran);
        FillWritable(source[3],ran);
        for(size_t j=source.cols;j>0;--j) source[2][j] = 3*source[1][j];
        std::cerr << "source=" << source << std::endl;

        Apex::Mylar::Compress(target,source);
        std::cerr << "target=" << target << std::endl;

    }


}
Y_UDONE()
