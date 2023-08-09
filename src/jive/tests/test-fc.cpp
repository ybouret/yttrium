#include "y/jive/pattern/segment.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Jive;

Y_UTEST(fc)
{
    Segment::List fc;
    Segment::AppendTo(fc, new Segment('a') );
    Segment::AppendTo(fc, new Segment('k') );

}
Y_UDONE()

