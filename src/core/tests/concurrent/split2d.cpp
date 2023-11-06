
#include "y/concurrent/tiles.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;



Y_UTEST(concurrent_split2d)
{

    for(unsigned size=1;size<=3;++size)
    {
        std::cerr << "size=" << size << std::endl;
        Concurrent::Tiles<int> tiles(size, V2D<int>(1,1), V2D<int>(10,20) );
    }



}
Y_UDONE()

