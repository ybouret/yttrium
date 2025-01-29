
#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {
        static inline
        natural_t z2n(const SignType  s,
                      const integer_t z) noexcept
        {
            switch(s)
            {
                case Negative: return -z;
                case Positive: return  z;
                case __Zero__: break;
            }
            assert(0==z);
            return 0;
        }

        Integer:: Integer(const integer_t z) :
        s( Sign::Of(z) ),
        n( z2n(s,z) )
        {
        }

        Integer:: ~Integer() noexcept
        {
        }
        

    }

}
