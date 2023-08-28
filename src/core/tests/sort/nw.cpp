
#include "y/sort/nw/tables.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;

Y_UTEST(sort_nw)
{

    std::cerr << "MaxSize=" << NetworkSort::Tables::MaxSize << std::endl;

    for(unsigned i=NetworkSort::Tables::MinSize;i<=NetworkSort::Tables::MaxSize;++i)
    {
        const NetworkSort::Table &table = NetworkSort::Tables::CxxData[i];
        std::cerr << "@" << std::setw(2) << i << " -> #swaps=" << std::setw(4) << table.swaps << std::endl;
    }

}
Y_UDONE()
