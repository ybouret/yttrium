
#include "y/concurrent/split.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;



Y_UTEST(concurrent_split1d)
{

    int Length = 100;
    int Offset = 0;

    if(argc>1) Length = ASCII::Convert::To<int>(argv[1],"Length");
    if(argc>2) Offset = ASCII::Convert::To<int>(argv[2],"Offset");




    for(size_t size=1;size<=3;++size)
    {
        std::cerr << "size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            int length = Length;
            int offset = Offset;

            Concurrent::Split::With(size, rank, length, offset);
            std::cerr << "  " << size << "." << rank << " : @" << offset << " +" << length << std::endl;
        }
    }


}
Y_UDONE()

