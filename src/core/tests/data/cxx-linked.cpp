
#include "y/data/list/cxx.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/data/list/cloneable.hpp"

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

        iNode * clone() const { return new iNode(*this); }

    private:
        Y_DISABLE_ASSIGN(iNode);
    };

    int iNode::Counter = 0;
}

Y_UTEST(data_cxx_linked)
{
    Random::Rand           ran;
    CxxListOf<iNode>       L;
    CxxPoolOf<iNode>       P;
    ListOfCloneable<iNode> C;

    for(size_t i=10+ran.leq(10);i>0;--i)
    {
        if(ran.choice()) L.pushTail( new iNode() ); else L.pushHead( new iNode() );
        P.store( new iNode() );
        if(ran.choice()) C.pushTail( new iNode() ); else C.pushHead( new iNode() );
    }

    {
        const CxxListOf<iNode> tmpL = L;
        const CxxPoolOf<iNode> tmpP = P;
        const ListOfCloneable<iNode> tmpC = C;
    }

}
Y_UDONE()


