#include "y/data/list/ordered.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"

using namespace Yttrium;

namespace
{
    class Node : public Object
    {
    public:

        const int data;

        inline explicit Node(const int a) noexcept : Object(), data(a), next(0), prev(0) {}
        inline virtual ~Node() noexcept {}

        inline const int & operator*() const noexcept { return data; }

        Node *next, *prev;

        class Comparator
        {
        public:
            inline  Comparator() noexcept {}
            inline ~Comparator() noexcept {}

            inline SignType operator()(const Node *lhs, const Node *rhs) const noexcept
            {
                return Sign::Of(lhs->data,rhs->data);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Comparator);
        };
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Node);
    };
}

#include "y/random/bits.hpp"
#include "y/ptr/auto.hpp"

Y_UTEST(data_ordered_list)
{
    Random::Rand ran;

    CxxPoolOf<Node>                                         npool;
    OrderedList<Node,Node::Comparator,OrderedListQueryHead> hlist;
    OrderedList<Node,Node::Comparator,OrderedListQueryTail> tlist;

    for(size_t i=10+ran.leq(10);i>0;--i)
    {
        const int data = ran.in<int>(1,100);
        npool.store( new Node(data) );
        hlist.store( new Node(data) );
        tlist.store( new Node(data) );
    }

    std::cerr << "npool=" << npool << std::endl;
    std::cerr << "hlist=" << hlist << std::endl;
    std::cerr << "tlist=" << tlist << std::endl;

    while(hlist.size>0)
    {
        const AutoPtr<Node> hnode = hlist.query();
        const AutoPtr<Node> tnode = tlist.query();
        std::cerr << "h: " << **hnode << " | t: " << **tnode << std::endl;
    }


}
Y_UDONE()


