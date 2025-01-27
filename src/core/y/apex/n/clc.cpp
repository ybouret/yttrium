
#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Apex
    {
        
        Natural Natural:: sqrt() const
        {
            const Natural &s = *this;
            if(s<=1) return *this;
            Natural x0 = *this; x0._shr();
            while(true) {
                Natural x1 = x0+Div(s,x0); x1._shr();
                if(x1>=x0) break;
                x0.xch(x1);
            }
            return x0;
        }

        Natural Natural:: abs()  const
        {
            return *this;
        }
    }

}
