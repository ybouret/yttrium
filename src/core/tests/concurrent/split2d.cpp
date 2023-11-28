
#include "y/concurrent/tiling.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/text/plural.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;



Y_UTEST(concurrent_split2d)
{

    int X = 10;
    int Y = 20;
    if(argc>1) X = ASCII::Convert::To<int>(argv[1],"X");
    if(argc>2) Y = ASCII::Convert::To<int>(argv[2],"Y");

    const V2D<int> area( Max<int>(X,1), Max<int>(Y,1) );


    std::cerr << "Splitting " << area << std::endl;


    UnitTestDisplay::Width = 40;

    Y_SIZEOF(Concurrent::Tiling<int8_t>::Segment);
    Y_SIZEOF(Concurrent::Tiling<int16_t>::Segment);
    Y_SIZEOF(Concurrent::Tiling<int32_t>::Segment);
    Y_SIZEOF(Concurrent::Tiling<int64_t>::Segment);

    std::cerr << std::endl;
    Y_SIZEOF(Concurrent::Tiling<int8_t>::Tile);
    Y_SIZEOF(Concurrent::Tiling<int16_t>::Tile);
    Y_SIZEOF(Concurrent::Tiling<int32_t>::Tile);
    Y_SIZEOF(Concurrent::Tiling<int64_t>::Tile);


    for(unsigned size=1;size<=8;++size)
    {
        std::cerr << "Splitting " << area << " in at most " << size << " tile" << Plural::s(size) << std::endl;
        std::cerr << "size="      << size << std::endl;

        Concurrent::Tiling<int>::Tiles tiles(size, V2D<int>(1,1), area );
        std::cerr << tiles << std::endl;

        for(const Concurrent::Tiling<int>::Tile *t = tiles->head; t; t=t->next )
        {
            AutoPtr< Concurrent::Tiling<int>::Tile > ptr = t->clone();
            std::cerr << "#tile=" << t->items << std::endl;
            size_t count = 0;
            for( Concurrent::Tiling<int>::Iterator it = t->begin(); it != t->end(); ++it)
            {
                ++count;
                std::cerr << "/" << *it;
            }
            std::cerr << std::endl;
            Y_CHECK(t->items==count);
        }
        std::cerr << std::endl;
    }


}
Y_UDONE()

