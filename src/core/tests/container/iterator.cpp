
#include "y/container/iterator/linear.hpp"
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/out-of-reach.hpp"

using namespace Yttrium;

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
}
Y_UDONE()

