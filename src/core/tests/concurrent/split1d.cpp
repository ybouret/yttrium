
#include "y/concurrent/split1d.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;



Y_UTEST(concurrent_split1d)
{

    size_t Offset = 0;
    size_t Length = 100;

    for(size_t size=1;size<=3;++size)
    {
        std::cerr << "size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            size_t offset = Offset;
            size_t length = Length;

            Concurrent::Split1D::With(size, rank, length, offset);
            std::cerr << "  " << size << "." << rank << " : @" << offset << " +" << length << std::endl;
        }
    }


}
Y_UDONE()

