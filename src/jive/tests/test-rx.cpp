#include "y/jive/pattern/regexp.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/jive/pattern.hpp"

using namespace Yttrium;

Y_UTEST(rx)
{
    if(argc>1)
    {
        AutoPtr<Jive::Pattern> P = Jive::RegExp::Compile(argv[1]);

    }
}
Y_UDONE()
