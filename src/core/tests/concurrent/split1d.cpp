
#include "y/concurrent/split/trek.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;


template <typename T>
static inline void ShowIndex(const T &i)
{
    std::cerr << i << std::endl;
}

Y_UTEST(concurrent_split1d)
{



    int head = 1;
    int tail = 10;
    int step = 2;
    if(argc>1) head = ASCII::Convert::To<int>(argv[1],"head");
    if(argc>2) tail = ASCII::Convert::To<int>(argv[2],"tail");
    if(argc>3) step = ASCII::Convert::To<int>(argv[3],"step");

    const size_t             sz   = 1;
    const size_t             rk   = 0;

    Concurrent::ForLoop<int> full = Concurrent::Split::For(sz,rk, head, tail, step);
    std::cerr << "full=" << full << std::endl;

    full.sweep(ShowIndex<int>);


    return 0;

    for(size_t size=1;size<=10;++size)
    {
        std::cerr << std::endl;
        std::cerr << "size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            Concurrent::ForLoop<int> trek = Concurrent::Split::For(size,rank, head, tail, step);
            std::cerr << "->" << trek << std::endl;
        }
    }

}
Y_UDONE()

