#include "y/apex/types.hpp"
#include "y/utest/run.hpp"
#include "y/system/rtti.hpp"
#include "y/calculus/ilog2.hpp"
#include "y/calculus/base2.hpp"


namespace Yttrium
{
    namespace Apex
    {

        struct API
        {
            static const size_t MinCapa = 2*sizeof(uint64_t);
            static const size_t MinLog2 = iLog2<MinCapa>::Value;

            template <typename T> static inline
            size_t CapacityTo(const size_t capa) noexcept
            {
                assert(IsPowerOfTwo(capa));
                assert(capa>=MinCapa);
                return capa >> ( iLog2Of<T>::Value );
            }


        };

    }
}

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_types)
{

    Y_USHOW(Apex::API::MinCapa);
    Y_USHOW(Apex::API::MinLog2);

    Y_USHOW(Apex::API::CapacityTo<uint8_t>(32));
    Y_USHOW(Apex::API::CapacityTo<uint16_t>(32));
    Y_USHOW(Apex::API::CapacityTo<uint32_t>(32));
    Y_USHOW(Apex::API::CapacityTo<uint64_t>(32));

}
Y_UDONE()


