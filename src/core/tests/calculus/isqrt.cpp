#include "y/calculus/isqrt.hpp"
#include "y/utest/run.hpp"

#include "y/io/libc/output.hpp"

using namespace Yttrium;

Y_UTEST(calculus_isqrt)
{

    Libc::OutputFile fp("isqrt.dat");
    for(int i=0;i<=65536;++i)
    {
        fp("%d %d\n", i, IntegerSquareRoot(i));
    }


}
Y_UDONE()


