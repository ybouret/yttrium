#include "y/apex/types.hpp"
#include "y/utest/run.hpp"
#include "y/system/rtti.hpp"
#include "y/calculus/ilog2.hpp"


namespace Yttrium
{
    namespace Apex
    {

        struct API
        {
            static const size_t MinSize = 2*sizeof(uint64_t);
            static const size_t MinLog2 = iLog2<MinSize>::Value;
            



        };

    }
}

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_types)
{

    Y_USHOW(Apex::API::MinSize);
    Y_USHOW(Apex::API::MinLog2);

}
Y_UDONE()


