
#include "y/concurrent/tiles.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/text/plural.hpp"

#include "y/utest/run.hpp"


using namespace Yttrium;



Y_UTEST(concurrent_split2d)
{

    Y_SIZEOF(Concurrent::Segment<int8_t>);
    Y_SIZEOF(Concurrent::Segment<int16_t>);
    Y_SIZEOF(Concurrent::Segment<int32_t>);
    Y_SIZEOF(Concurrent::Segment<int64_t>);



    int X = 10;
    int Y = 20;
    if(argc>1) X = ASCII::Convert::To<int>(argv[1],"X");
    if(argc>2) Y = ASCII::Convert::To<int>(argv[2],"Y");

    const V2D<int> area( Max<int>(X,1), Max<int>(Y,1) );
    std::cerr << "Splitting " << area << std::endl;

#if 0
    for(unsigned size=1;size<=8;++size)
    {
        std::cerr << "Splitting " << area << " in at most " << size << " tile" << Plural::s(size) << std::endl;
        std::cerr << "size="      << size << std::endl;
        Concurrent::Tiles<int> tiles(size, V2D<int>(1,1), area );
    }
#endif

    Concurrent::Tile<int> tile(12);

    tile.push(area, 10);
    tile.push(area, 12);
    std::cerr << "#items=" << tile.items << std::endl;
    std::cerr << tile << std::endl;

}
Y_UDONE()

