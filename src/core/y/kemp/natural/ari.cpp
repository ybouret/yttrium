#include "y/kemp/natural.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
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


        Natural Natural:: LCM(const Natural &a, const Natural &b)
        {

            assert(a>0); assert(b>0);
            return (a*b)/__GCD(a,b);
        }

        Natural Natural:: Factorial(size_t n)
        {
            if(n<=1)
            {
                return Natural(1);
            }
            else
            {
                assert(n>1);
                Natural f(2);
                for(size_t i=3;i<=n;++i)
                {
                    const Natural x(i);
                    f *= x;
                }
                return f;
            }
        }

        Natural Natural:: Comb(const size_t n, const size_t k)
        {
            assert(k<=n);
            const Natural numer = Factorial(n);
            const Natural lhs   = Factorial(k);
            const Natural rhs   = Factorial(n-k);
            const Natural denom = lhs*rhs;
            return numer/denom;
        }

        Natural Natural:: Sqrt(const Natural &s)
        {
            if(s<=1) return s;
            Natural x0 = s; x0.shr();
            while(true) {
                Natural x1 = (x0+s/x0);
                x1.shr();
                if(x1>=x0) break;
                x0.xch(x1);
            }
            return x0;
        }

        Natural Natural:: sqrt() const
        {
            return Sqrt(*this);
        }

        void Natural:: Simplify(Natural &num, Natural &den)
        {
            if(den<=0) throw Specific::Exception(CallSign, "Simplify with zero denominator");
            if(num<=0) { den=1; return; }
            if(1==den) return;

            assert(num>0);
            assert(den>1);
            const Natural g = __GCD(num,den);
            Natural n = num/g;
            Natural d = den/g;

            num.xch(n);
            den.xch(d);


        }
    }

}

