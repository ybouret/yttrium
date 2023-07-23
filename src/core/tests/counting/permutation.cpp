
#include "y/counting/permutation.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(counting_permutation)
{
    for(size_t n=1;n<=8;++n)
    {
        std::cerr << "n=" << n << std::endl;
        Permutation perm(n);
        size_t      count = 0;
        do
        {
            std::cerr << perm << std::endl;
            ++count;
        } while( perm.next() );
        Y_ASSERT(count==perm.total);
        perm.boot();

        count = 0;
        do
        {
            ++count;
        } while( perm.next() );
        Y_ASSERT(count==perm.total);

    }

}
Y_UDONE()

