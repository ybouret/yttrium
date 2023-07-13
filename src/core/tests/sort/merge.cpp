
#include "y/sort/merge.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/object.hpp"
#include "y/data/list/cxx.hpp"
#include "y/comparison.hpp"

using namespace Yttrium;

namespace
{
    template <typename T>
    class XNode : public Object
    {
    public:
        XNode *next;
        XNode *prev;
        T      data;

        explicit XNode(const T value) : next(0), prev(0), data(value) {}
        virtual ~XNode() noexcept {}

        const T & operator*() const noexcept { return data; }

        static inline SignType Compare(const XNode *lhs,
                                       const XNode *rhs) noexcept
        {
            return Comparison::CxxIncreasing(**lhs,**rhs);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XNode);
    };
}


Y_UTEST(sort_merge)
{
    Random::Rand  ran;

    CxxListOf< XNode<int> > L;
    for(int i=0; i <= 23; ++i) L.pushTail( new XNode<int>(i) );

    std::cerr << "Init = " << L << std::endl;

    Random::Shuffle::List(L,ran);
    std::cerr << "Rand = " << L << std::endl;

    MergeSort::Call(L, XNode<int>::Compare );
    std::cerr << "Sort = " << L << std::endl;

    Random::Shuffle::List(L,ran);

    MergeSort:: ByIncreasingAddress(L);
    std::cerr << "Addr = " << L << std::endl;


}
Y_UDONE()

