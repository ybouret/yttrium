#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Apex
    {

        static inline
        Natural __GCD(const Natural &A, const Natural &B)
        {
            Natural a = A;
            Natural b = B;
            if(a<b) a.xch(b);
            assert(a>=b);
            while(b>0)
            {
                Natural r = a%b;
                a.xch(b);
                b.xch(r);
            }
            return a;
        }

        Natural Natural:: GCD(const Natural &a, const Natural &b)
        {
            if(a<=0)
            {
                if(b<=0)
                {
                    return 1;
                }
                else
                {
                    return b;
                }
            }
            else
            {
                assert(a>0);
                if(b<=0)
                {
                    return a;
                }
                else
                {
                    assert(b>0);
                    return __GCD(a,b);
                }
            }
        }

        void Natural:: Simplify(Natural &Numer, Natural &Denom)
        {
            if(Denom<=0) throw Specific::Exception(CallSign,"Simplify by Zero");
            if(Numer==0)
            {
                Denom = 1;
            }
            else
            {
                const Natural g = __GCD(Numer,Denom);
                assert(g>0);
                if(1!=g)
                {
                    Numer /= g;
                    Denom /= g;
                }
            }
        }
    }

}

