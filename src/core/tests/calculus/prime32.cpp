#include "y/calculus/prime.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include "y/stream/libc/output.hpp"
#include "y/calculus/bit-count.hpp"

using namespace Yttrium;

static int width = 12;

Y_UTEST(calculus_prime32)
{
    size_t   count = 2; // 2 and 3
    uint64_t p = Prime64::Next(3);
    std::cerr << std::setw(width) << p << std::endl;
    uint64_t zmax = 0;
    uint64_t prev = 0;
    uint64_t next = 0;
    OutputFile::Overwrite("zprime.dat");

    while( true )
    {
        const uint64_t q = Prime64::Next(p+1);
        const uint64_t d = q-p;  Y_ASSERT(0==(d&1));
        const uint64_t h = d>>1; Y_ASSERT(h>0);
        const uint64_t z = h-1;

        if(z>zmax)
        {
            zmax = z;
            prev = p;
            next = q;
        }

        if(BitsFor(z)>=8)
        {
            std::cerr << "*** z=" << z << std::endl;
            break;
        }

        {
            AppendFile fp("zprime.dat");
            fp("%g %g %g %g\n", double(q), double(z), double (zmax), double(BitsFor(zmax)) );
        }
        std::cerr << std::setw(width) << q << " @" << std::setw(width) << z << std::endl;
        p=q;
        ++count;
    }
    std::cerr << "zmax=" << zmax << " between " << prev << " and " << next << std::endl;
    std::cerr << "count = " << count << std::endl;
}
Y_UDONE()

