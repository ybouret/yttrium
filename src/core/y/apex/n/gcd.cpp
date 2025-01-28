#include "y/apex/natural.hpp"


namespace Yttrium
{
    namespace Apex
    {
        Natural Natural:: GCD_(const Natural &a, const Natural &b)
        {
            assert(a>0);
            assert(b>0);

            Natural A = a;
            Natural B = b;

            if(A<B)
                A.xch(B);

            assert(A>=B);
            while(B->bits>0)
            {
                Natural r = Mod(A,B);
                A.xch(B);
                B.xch(r);
            }

            return A;
        }

        Natural Natural::GCD(const Natural &a, const Natural &b)
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
                    return GCD_(a,b);
                }
            }
        }

    }

}

