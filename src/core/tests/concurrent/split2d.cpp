
#include "y/concurrent/tiles.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/text/plural.hpp"

#include "y/utest/run.hpp"


using namespace Yttrium;



Y_UTEST(concurrent_split2d)
{

    Y_SIZEOF( Concurrent::Segment<int> );
    Y_SIZEOF( Concurrent::Tile<int>    );
    Y_SIZEOF( Concurrent::Tiles<int>   );


    int X = 10;
    int Y = 20;
    if(argc>1) X = ASCII::Convert::To<int>(argv[1],"X");
    if(argc>2) Y = ASCII::Convert::To<int>(argv[2],"Y");

    const V2D<int> area(X,Y);
    std::cerr << "Splitting " << area << std::endl;

    for(unsigned size=1;size<=3;++size)
    { 
        std::cerr << "Splitting " << area << " in at most " << size << " tile" << Plural::s(size) << std::endl;

        std::cerr << "size=" << size << std::endl;
        Concurrent::Tiles<int> tiles(size, V2D<int>(1,1), area );
    }



}
Y_UDONE()

