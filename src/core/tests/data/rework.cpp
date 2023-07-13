
#include "y/data/rework.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/data/list/cxx.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/object.hpp"

using namespace Yttrium;

namespace {

    class iNode : public Object
    {
    public:
        iNode *next, *prev;
        int           data;
        const int     mark;

        static int Count;

        virtual ~iNode() noexcept { --Count; }
        explicit iNode() noexcept : next(0), prev(0), data(++Count), mark(data) {}


        inline const int & operator*() const noexcept { return data; }
        iNode(const iNode &other) : next(0), prev(0), data(other.data), mark(++Count) {}


    private:
        Y_DISABLE_ASSIGN(iNode);
    };

    int iNode::Count = 0;

}

Y_UTEST(data_rework)
{
    Random::Rand ran;

    for(size_t n=0;n<=10;++n)
    {
        CxxPoolOf<iNode> P;

        for(size_t i=0;i <n; ++i) P.store( new iNode() );
        Random::Shuffle::Pool(P,ran);
        std::cerr << "Pool=" << P << std::endl;

        const CxxPoolOf<iNode> Q(P);
        std::cerr << "Copy=" << Q << std::endl;

        CxxListOf<iNode> L;

        Rework::PoolToList(L,P);
        std::cerr << "List=" << L << std::endl;
        L.reverse();
        std::cerr << "List=" << L << std::endl;
        L.reverse();
        std::cerr << "List=" << L << std::endl;


    }



}
Y_UDONE()
