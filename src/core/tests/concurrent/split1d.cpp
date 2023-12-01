
#include "y/concurrent/split/divide.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;



Y_UTEST(concurrent_split1d)
{


    Y_SIZEOF(Concurrent::TrekOf<int8_t>);
    Y_SIZEOF(Concurrent::TrekOf<int16_t>);
    Y_SIZEOF(Concurrent::TrekOf<int32_t>);
    Y_SIZEOF(Concurrent::TrekOf<int64_t>);


    int head = 1;
    int tail = 3;
    int step = 1;
    if(argc>1) head = ASCII::Convert::To<int>(argv[1],"head");
    if(argc>2) tail = ASCII::Convert::To<int>(argv[2],"tail");
    if(argc>3) step = ASCII::Convert::To<int>(argv[3],"step");



    for(size_t size=1;size<=10;++size)
    {
        std::cerr << std::endl;
        std::cerr << "size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            Concurrent::TrekOf<int> trek = Concurrent::Divide::For(size,rank, head, tail, step);
            std::cerr << "->" << trek << std::endl;
        }
    }

}
Y_UDONE()

