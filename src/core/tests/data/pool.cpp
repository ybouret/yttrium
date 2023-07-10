
#include "y/data/pool.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

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

    Random::Rand  ran;
    PoolOf<iNode> iPool;

    std::cerr << "store" << std::endl;
    for(size_t i=ran.leq(30)+2;i>0;--i)
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

