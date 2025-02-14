
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


        bool Natural:: isOdd() const noexcept
        {
            return block->curr->isOdd();
        }

        bool Natural:: isEven()  const noexcept
        {
            return !isOdd();
        }


        Natural Natural:: Factorial(const natural_t n)
        {
            switch(n)
            {
                case 0:
                case 1:
                    return 1;
                default:
                    break;
            }
            Natural f = 2;
            for(natural_t i=3;i<=n;++i)
            {
                f *= i;
            }
            return f;
        }
    }

}
