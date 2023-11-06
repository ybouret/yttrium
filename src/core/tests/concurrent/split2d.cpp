
#include "y/concurrent/tiles.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;



Y_UTEST(concurrent_split2d)
{
    int X = 10;
    int Y = 20;
    if(argc>1) X = ASCII::Convert::To<int>(argv[1],"X");
    if(argc>2) Y = ASCII::Convert::To<int>(argv[2],"Y");

    for(unsigned size=1;size<=3;++size)
    {
        std::cerr << "size=" << size << std::endl;
        Concurrent::Tiles<int> tiles(size, V2D<int>(1,1), V2D<int>(X,Y) );
    }



}
Y_UDONE()

