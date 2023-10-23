#include "y/data/list.hpp"
#include "y/memory/out-of-reach.hpp"

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
        virtual ~iNode() noexcept { --Counter; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(iNode);
    };

    int iNode::Counter = 0;
}

Y_UTEST(data_list)
{
    Random::Rand  ran;
    ListOf<iNode> iList;
    for(size_t i=10+ran.leq(10);i>0;--i)
    {
        if( ran.choice() )
            iList.pushTail( new iNode() );
        else
            iList.pushHead( new iNode() );
    }
    std::cerr << "#=" << iList.size << std::endl;
    std::cerr << iList << std::endl;
    while(iList.size)
    {
        delete ( ran.choice() ? iList.popTail() : iList.popHead());
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
        Random::Shuffle::Range(indx,n,ran);
        for(size_t i=0;i<n;++i)
        {
            iNode *node = &nodes[ indx[i] ];
            std::cerr << "using node #" << **node << std::endl;
            ListOps::InsertOrdered(iList,node, ListOps::IncreasingAddresses<iNode>);
            std::cerr << iList << std::endl;
        }

        while(iList.size) iList.popTail();
        Random::Shuffle::Range(indx,n,ran);

        for(size_t i=0;i<n;++i)
        {
            iNode *node = &nodes[ indx[i] ];
            std::cerr << "using node #" << **node << std::endl;
            iList.insertByIncreasingAddress(node);
            Y_ASSERT( ListOps::CheckIncreasingAddresses(iList) );
            std::cerr << iList << std::endl;
        }

        ListOf<iNode> lhs, rhs;
        ListOps:: Divide(lhs,rhs,iList);
        std::cerr << "lhs = " << lhs << std::endl;
        std::cerr << "rhs = " << rhs << std::endl;

        ListOps:: Fusion(iList, rhs, lhs,  ListOps::IncreasingAddresses<iNode> );
        std::cerr << "tgt = " << iList << std::endl;
        
        while(iList.size) iList.popTail();

        Y_SIZEOF(ListOf<iNode>);

    }

    {
        std::cerr << "MoveToFront" << std::endl;
        for(size_t n=1;n<=10;++n)
        {
            for(size_t i=0;i<n;++i)
            {
                if( ran.choice()  )
                    iList.pushTail( new iNode() );
                else
                    iList.pushHead( new iNode() );
            }
            Y_ASSERT(n==iList.size);

            for(size_t loop=0;loop<n;++loop)
            {
                iNode *node = ListOps::Next(iList.head,loop);
                Y_ASSERT(0!=node);
                iList.moveToFront(node);
            }

            for(size_t i=1;i<=iList.size;++i)
            {
                const ListOf<iNode> &cL = iList;
                iNode       *lhs = iList.fetch(i);
                const iNode *rhs = cL.fetch(i);
                Y_ASSERT(lhs==rhs);
            }

            while(iList.size) delete iList.popTail();
        }
    }

    {
        std::cerr << "Insert After/Before" << std::endl;
        for(size_t cycle=0;cycle<=1000;++cycle)
        {
            for(size_t n=1;n<=10;++n)
            {
                for(size_t i=0;i<n;++i)
                {
                    if( ran.choice()  )
                        iList.pushTail( new iNode() );
                    else
                        iList.pushHead( new iNode() );
                }
                Y_ASSERT(n==iList.size);

                for(size_t iter=0;iter<10;++iter)
                {
                    iNode *mine = iList.fetch( ran.in<size_t>(1,iList.size) );
                    Y_ASSERT(iList.owns(mine));
                    iNode *node = new iNode();
                    if( ran.choice() )
                    {
                        iList.insertAfter(mine,node);
                        Y_ASSERT(mine->next == node);
                        Y_ASSERT(node->prev == mine);
                    }
                    else
                    {
                        iList.insertBefore(mine,node);
                        Y_ASSERT(mine->prev == node);
                        Y_ASSERT(node->next == mine);
                    }
                }

                while(iList.size) delete iList.popTail();
            }
        }
    }

}
Y_UDONE()

