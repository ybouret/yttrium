#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        void Natural:: Div(Natural &quot, Natural &rem, const Natural &num, const Natural &den)
        {
            DivModArg retBoth = { &rem };
            quot = DivMod(num,den,&retBoth);
            
            //quot = Div(num,den);
            //rem  = num-quot * den;
        }

    }

}

