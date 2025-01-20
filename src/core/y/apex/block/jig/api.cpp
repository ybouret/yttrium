
#include "y/apex/block/jig/api.hpp"
#include "y/calculus/base2.hpp"

namespace Yttrium
{
    namespace Apex
    {
        JigAPI:: JigAPI(const size_t _count) noexcept :
        words(0), count(_count)
        {
            assert(IsPowerOfTwo(count));
        }

        JigAPI:: ~JigAPI() noexcept
        {
        }



        const Plan JigAPI:: Dull[Plans][Faded] =
        {
            { Plan2, Plan4, Plan8 },
            { Plan1, Plan4, Plan8 },
            { Plan1, Plan2, Plan8 },
            { Plan1, Plan2, Plan4 },
        };



        std::ostream & operator<<(std::ostream &os, const JigAPI &J)
        {
            J.display(os);
            return os;
        }


    }

}

