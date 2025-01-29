
#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {
        Integer Integer:: operator-() const
        {
            Integer _(*this);
            Sign::ReplaceByOpposite( Coerce(_.s) );
            return _;
        }

    }

}
