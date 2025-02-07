
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: operator-() const
        {
            Rational _(*this);
            Sign::ReplaceByOpposite( Coerce(_.numer.s) );
            return _;
        }

    }

}
