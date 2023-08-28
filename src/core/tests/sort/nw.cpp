
#include "y/sort/nw.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/mkl/v3d.hpp"

using namespace Yttrium;

Y_UTEST(sort_nw)
{

    std::cerr << "MaxSize=" << NetworkSort::Tables::MaxSize << std::endl;

    for(unsigned i=NetworkSort::Tables::MinSize;i<=NetworkSort::Tables::MaxSize;++i)
    {
        const NetworkSort::Table &table = NetworkSort::Tables::CxxData[i];
        std::cerr << "@" << std::setw(2) << i << " -> #swaps=" << std::setw(4) << table.swaps << std::endl;
    }

    NetworkSort::Common::Display<2>();
    NetworkSort::Common::Display<3>();
    NetworkSort::Common::Display<4>();
    NetworkSort::Common::Display<5>();
    NetworkSort::Common::Display<6>();
    NetworkSort::Common::Display<7>();
    NetworkSort::Common::Display<8>();
    NetworkSort::Common::Display<9>();
    NetworkSort::Common::Display<10>();
    NetworkSort::Common::Display<11>();
    NetworkSort::Common::Display<12>();
    //NetworkSort::Display<13>();


    V3D<double> v3d(3,-1,2);

    std::cerr << v3d << " -> ";
    NetworkSort::Algo<3,double>::Increasing(v3d);
    std::cerr << v3d << " -> ";
    NetworkSort::Algo<3,double>::Decreasing(v3d);
    std::cerr << v3d << std::endl;
}
Y_UDONE()
