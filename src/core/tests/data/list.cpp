#include "y/data/list.hpp"
#include "y/memory/out-of-reach.hpp"

#include "y/utest/run.hpp"
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
        virtual ~iNode() noexcept { --Counter; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(iNode);
    };

    int iNode::Counter = 0;
}

Y_UTEST(data_list)
{

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

    {
        static const size_t n = 20;
        iNode  nodes[n];
        size_t indx[n];
        for(size_t i=0;i<n;++i)
        {
            indx[i] = i;
        }
        alea_shuffle(indx,n);
        for(size_t i=0;i<n;++i)
        {
            iNode *node = &nodes[ indx[i] ];
            std::cerr << "using node #" << **node << std::endl;
            ListOps::InsertOrdered(iList,node, ListOps::IncreasingAddresses<iNode>);
            std::cerr << iList << std::endl;
        }

        while(iList.size) iList.popTail();
        //Memory::OutOfReach::Zero(&iList,sizeof(iList));
        alea_shuffle(indx,n);

        for(size_t i=0;i<n;++i)
        {
            iNode *node = &nodes[ indx[i] ];
            std::cerr << "using node #" << **node << std::endl;
            iList.pushByAddr(node);
            std::cerr << iList << std::endl;
        }

        while(iList.size) iList.popTail();



    }
    
}
Y_UDONE()

