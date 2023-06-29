
#include "y/data/pool.hpp"
#include "y/utest/run.hpp"
#include "y/check/crc32.hpp"
#include "../alea.hpp"

using namespace Yttrium;

namespace
{
    class iNode
    {
    public:
        iNode *next;
        iNode *prev;
        int    data;

        inline int operator*() const noexcept { return data; }
        static int Counter;

        explicit iNode() noexcept : next(0), prev(0),  data(++Counter) {}
        virtual ~iNode() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(iNode);
    };

    int iNode::Counter = 0;
}

Y_UTEST(data_pool)
{
    const unsigned seed = unsigned(time(0));
    srand( CRC32::Of(seed) );

    PoolOf<iNode> iPool;

    std::cerr << "store" << std::endl;
    for(size_t i=alea_leq(30)+2;i>0;--i)
    {
        iPool.store( new iNode() );
        std::cerr << iPool << std::endl;
    }

    std::cerr << "query" << std::endl;
    while(iPool.size)
    {
        delete iPool.query();
       std::cerr << iPool << std::endl;
    }

}
Y_UDONE()

