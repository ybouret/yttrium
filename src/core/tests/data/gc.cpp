
#include "y/data/gc/cxx-pool.hpp"

#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/object.hpp"

using namespace Yttrium;

namespace
{
    class iNode : public Object
    {
    public:
        iNode *next;
        iNode *prev;
        const int    count;
        const int    value;

        inline int operator*() const noexcept { return value; }
        static int Counter;

        explicit iNode() noexcept : Object(), next(0), prev(0),  count(++Counter), value(count) {}
        virtual ~iNode() noexcept { --Counter; }

        iNode(const iNode &other ) noexcept : Object(), next(0), prev(0), count(++Counter), value(other.value) {}

        //iNode * clone() const { return new iNode(*this); }

    private:
        Y_DISABLE_ASSIGN(iNode);
    };

    int iNode::Counter = 0;
}

Y_UTEST(data_gc)
{
    Random::Rand           ran;
    CxxListOf<iNode>       L;
    CxxPoolOf<iNode>       P;

    for(size_t i=10+ran.leq(10);i>0;--i)
    {
        if(ran.choice()) L.pushTail( new iNode() ); else L.pushHead( new iNode() );
        P.store( new iNode() );
    }

    std::cerr << "L=" << L << std::endl;
    CxxListGC:: Cycle(L,2);
    std::cerr << "L=" << L << std::endl;
    Y_CHECK(ListOps::CheckIncreasingAddresses(L));

    std::cerr << "P=" << P << std::endl;
    CxxPoolGC:: Cycle(P,1);
    std::cerr << "P=" << P << std::endl;
    Y_CHECK(ListOps::CheckIncreasingAddresses(P));




}
Y_UDONE()

