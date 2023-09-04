#include "y/ordered/vector.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/container/cxx-array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/container/iterator/display-all.hpp"
#include "../main.hpp"

using namespace Yttrium;

namespace {

    template <typename ITERATOR>
    static inline void DisplayRange(ITERATOR curr, const ITERATOR last)
    {
        while(curr!=last)
        {
            std::cerr << ' ' << *curr;
            ++curr;
        }
        std::cerr << std::endl;
    }

    class StringComparator
    {
    public:
        inline  StringComparator() noexcept {}
        inline ~StringComparator() noexcept {}

        inline SignType operator()(const String &lhs, const String &rhs) const noexcept
        {
            return String::Compare(lhs,rhs);
        }

    };
}

Y_UTEST(ordered_vector)
{
    Random::Rand ran;

    { OrderedVector<int> V0; }

    {
        OrderedVector<int,IncreasingComparator,Memory::Dyadic> V(5);


        CxxArray<int,Memory::Pooled> data(10 + ran.leq(22));
        for(size_t i=data.size();i>0;--i) data[i] = int(i);
        std::cerr << "data=" << data << std::endl;

        V.free();
        {
            for(size_t i=data.size();i>0;--i)
            {
                Y_ASSERT( V.insert( data[i] ) );
            }
            std::cerr << V << std::endl;
            Iterating::DisplayAll::ConstOf(V);
        }

    }

    {
        OrderedVector<String,StringComparator,Memory::Pooled> V;
        for(size_t i = 5+ran.leq(5);i>0;--i)
        {
        TRIAL:
            const String tmp = Bring<String>::Get(ran);
            if(!V.insert(tmp)) goto TRIAL;
        }
        std::cerr << V << std::endl;
    }


}
Y_UDONE()

