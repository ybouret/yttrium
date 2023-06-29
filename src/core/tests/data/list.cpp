#include "y/data/list.hpp"
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

Y_UTEST(data_list)
{
    const unsigned seed = unsigned(time(0));
    srand( CRC32::Of(seed) );

    ListOf<iNode> iList;
    for(size_t i=10+alea_leq(10);i>0;--i)
    {
        if( alea() > 0.5 )
            iList.pushTail( new iNode() );
        else
            iList.pushHead( new iNode() );
    }
    std::cerr << "#=" << iList.size << std::endl;
    std::cerr << iList << std::endl;
    while(iList.size)
    {
        delete ( (alea()>0.5) ? iList.popTail() : iList.popHead());
        std::cerr << iList << std::endl;
    }
    
}
Y_UDONE()

