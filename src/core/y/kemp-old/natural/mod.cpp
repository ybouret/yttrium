
#include "y/kemp/natural.hpp"
#include "y/system/exception.hpp"
#include "y/kemp/element.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Kemp
    {


        Natural & Natural:: operator%=(const Natural &rhs)
        {
            Natural res = Modulo(*this,rhs);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator%=(const uint64_t rhs)
        {
            const Natural den(rhs);
            return *this %= den;
        }


        Natural operator%(const Natural &lhs, const Natural &rhs)
        {
            return Natural::Modulo(lhs,rhs);
        }

        Natural operator%(const uint64_t lhs, const Natural &rhs)
        {
            const Natural num(lhs);
            return Natural::Modulo(num,rhs);
        }

        Natural operator%(const Natural &lhs, const uint64_t rhs)
        {
            const Natural den(rhs);
            return Natural::Modulo(lhs,den);
        }


#if 0
        // num = q*den (+rem)
        Natural Natural:: Modulo(const Natural &num, const Natural &den)
        {
            const Natural q = Divide(num,den);
            return num - q*den;
        }

#else

        Natural Natural:: Modulo(const Natural &num, const Natural &den)
        {
            //__________________________________________________________________
            //
            //
            // get and check elements
            //
            //------------------------------------------------------------------
            Element &D = *den; if(D.bits<=0) throw Libc::Exception(EDOM, "%s modulo by zero", CallSign);
            Element &N = *num;

            switch( Element::Compare(N,D) )
            {
                case Negative: return num;                     // N < D
                case __Zero__: return Natural();               // N == D
                case Positive: assert(N.bits>=D.bits); break;  // N > D
            }


            //__________________________________________________________________
            //
            //
            // 2^p * den < num < 2^(p+1) * den
            //
            //__________________________________________________________________
            AutoPtr<Natural> lower = 0;
            AutoPtr<Natural> upper = 0;
            {
                size_t           p     = N.bits-D.bits;
                Natural          probe = den << p;
                switch( Compare(probe,num) )
                {
                    case Negative:
                        lower = new Natural(Exp2,p);   assert(*lower*den<num);
                        upper = new Natural(Exp2,++p); assert(num<*upper*den);
                        break;
                    case __Zero__: return Natural(); // den * 2^p = num
                    case Positive:
                        upper = new Natural(Exp2,p);   assert(num<*upper*den);
                        lower = new Natural(Exp2,--p); assert(*lower*den<num);
                        break;
                }
            }


            //__________________________________________________________________
            //
            //
            // bracket fom lo * den < num < up * den
            //
            //__________________________________________________________________
            Natural &lo = *lower;
            Natural &up = *upper;

            assert(lo*den<num);
            assert(num<up*den);

            while(true)
            {
                {
                    const Natural del = up-lo;
                    if(del._leq(1))
                    {
                        return num - lo * den;
                    }
                }

                Natural        mid  = (lo+up).shr();
                const Natural probe = mid * den;

                switch( Compare(probe,num) )
                {
                    case Negative: lo.xch(mid); break;
                    case __Zero__: return Natural(); // num = mid * den
                    case Positive: up.xch(mid); break;
                }

            }
        }
        
#endif

    }
}
