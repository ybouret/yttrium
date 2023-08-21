#include "y/jive/pattern/regexp.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/jive/pattern.hpp"
#include "y/system/exception.hpp"

using namespace Yttrium;

Y_UTEST(rx)
{
    if(argc>1)
    {
        AutoPtr<Jive::Pattern> P = Jive::RegExp::Compile(argv[1]);
    }

    throw Specific::Exception("whatever","here=%s",__FILE__);
}
Y_UDONE()
