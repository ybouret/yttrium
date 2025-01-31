
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        size_t Rational:: serialize(OutputStream &fp) const
        {
            const size_t n = numer.serialize(fp);
            return n + denom.serialize(fp);
        }
    }

}
