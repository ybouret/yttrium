#include "y/calculus/isqrt.hpp"
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"

#include "y/io/libc/output.hpp"

using namespace Yttrium;

Y_UTEST(calculus_isqrt)
{

    Libc::OutputFile fp("isqrt.dat");
    for(unsigned i=0;i<=65536;++i)
    {
        const unsigned s = IntegerSquareRoot(i);
        std::cerr << i << " -> " << s << std::endl;
        Y_ASSERT(s*s<=i);
        fp("%d %d\n", i, IntegerSquareRoot(i));
        const apn I = i;
        const apn S = I.sqrt();
        Y_ASSERT(S==s);
    }


}
Y_UDONE()


