
#include "y/container/cxx/array.hpp"
#include "y/container/iterator/linked.hpp"

#include "y/utest/run.hpp"
#include "y/kemp/natural.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/data/list/cxx.hpp"

using namespace Yttrium;

namespace
{
    class iNode : public Object
    {
    public:
        static int Count;

        int       count;
        iNode    *next;
        iNode    *prev;

        inline explicit iNode() noexcept : count(++Count), next(0), prev(0) {}
        inline virtual ~iNode() noexcept { --Count; }

        int &       operator*()       noexcept { return count; }
        const int & operator*() const noexcept { return count; }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(iNode);
    };

    int iNode:: Count = 0;
}

Y_UTEST(container_iterator)
{

    {
        int arr[4] = {1,2,3,4};

        Iterating::Linear<int,Iterating::Forward>       fwd = arr;
        Iterating::Linear<const int,Iterating::Reverse> rev = arr+3;
        for(size_t i=0;i<4;++i,++fwd,++rev)
        {
            std::cerr << *fwd << " " << *rev << std::endl;
        }

        fwd=arr+3;
        rev=arr;
        for(size_t i=0;i<4;++i,--fwd,--rev)
        {
            std::cerr << *fwd << " " << *rev << std::endl;
        }
    }

    {
        CxxListOf<iNode> L;
        for(size_t i=0;i<6;++i) L.pushTail( new iNode() );

        Iterating::Linked<int,iNode,Iterating::Forward> fwd = L.head;
    }

    
}
Y_UDONE()

