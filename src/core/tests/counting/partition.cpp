

#include "y/counting/partition.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(counting_partition)
{

    for(size_t n=1;n<=16;++n)
    {

        const apn np = Partition::Cardinal(n);
        std::cerr << "n=" << n << " => " << np << std::endl;

        Partition part(n);
        {
            size_t    count = 0;
            do
            {
                std::cerr << part << std::endl;
                ++count;
            } while(part.next());
            Y_CHECK(part.total==count);
        }
        part.boot();
        {
            size_t    count = 0;
            do
            {
                ++count;
            } while(part.next());
            Y_CHECK(part.total==count);
        }

    }
}
Y_UDONE()

