#include "y/apex/types.hpp"
#include "y/utest/run.hpp"
#include "y/system/rtti.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/allocator/archon.hpp"

namespace Yttrium
{
    namespace Apex
    {

        struct API
        {
            typedef uint64_t  natural_t;
            typedef int64_t   integer_t;

            static const size_t ConstantStaticBytes = 2 * sizeof(natural_t);
            static const size_t MinimalDynamicBytes = 2 * ConstantStaticBytes;
            

            template <typename T>
            struct NumberOf
            {
                static inline size_t ToHold(const size_t bytes) noexcept
                {
                    return Y_ALIGN_TO(T,bytes)/sizeof(T);
                }
            };




        };



    }
}

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_types)
{

    Y_USHOW(Apex::API::NumberOf<uint8_t> ::ToHold(21));
    Y_USHOW(Apex::API::NumberOf<uint16_t>::ToHold(21));
    Y_USHOW(Apex::API::NumberOf<uint32_t>::ToHold(21));
    Y_USHOW(Apex::API::NumberOf<uint64_t>::ToHold(21));


}
Y_UDONE()


