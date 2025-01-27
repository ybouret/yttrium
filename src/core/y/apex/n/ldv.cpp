#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        void Natural:: LDiv(Natural &quot, Natural &rem, const Natural &num, const Natural &den)
        {
            quot = Div(num,den);
            rem  = num-quot * den;
        }

    }

}

