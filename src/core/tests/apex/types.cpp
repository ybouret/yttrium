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
            static const size_t MinCapacity = 2*sizeof(uint64_t);
            static const size_t MinCapaLog2 = iLog2<MinCapacity>::Value;

            template <typename T> static inline
            size_t NumberOf(const size_t bytes) noexcept
            {
                assert(IsPowerOfTwo(bytes));
                assert(bytes>=MinCapacity);
                return bytes >> ( iLog2Of<T>::Value );
            }


        };

    }
}

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_types)
{

    Y_USHOW(Apex::API::MinCapacity);
    Y_USHOW(Apex::API::MinCapaLog2);

    Y_USHOW(Apex::API::NumberOf<uint8_t>(32));
    Y_USHOW(Apex::API::NumberOf<uint16_t>(32));
    Y_USHOW(Apex::API::NumberOf<uint32_t>(32));
    Y_USHOW(Apex::API::NumberOf<uint64_t>(32));

}
Y_UDONE()


