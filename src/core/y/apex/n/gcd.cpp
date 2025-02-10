#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"
#include <cerrno>

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
            if(a->bits<=0)
            {
                if(b->bits<=0)
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
                if(b->bits<=0)
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

        void Natural:: Simplify(Natural &num, Natural &den)
        {
            switch(den->bits)
            {
                case 0: assert(0==den); throw Libc::Exception(EDOM,"Natural Simplify by Zero");
                case 1: assert(1==den); return;
                default:
                    break;
            }
            assert(den>1);

            if(num->bits<=1)
            {
                den = 1;
            }
            else
            {
                assert(num>1);
                const Natural g = GCD_(num,den);
                if(g->bits>1)
                {
                    assert(g>1);
                    Natural N = num/g;
                    Natural D = den/g;
                    N.xch(num);
                    D.xch(den);
                }
            }
        }


        Natural Natural:: LCM(const Natural &a, const Natural b)
        {
            // std::cerr << "LCM(" << a << "," << b << ")" << std::endl;
            // std::cerr << "|_GCD=" << GCD(a,b) << std::endl;
            return (a*b)/GCD(a,b);

        }
    }

}

