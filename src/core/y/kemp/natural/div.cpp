#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Kemp
    {

        void Natural::Div(Natural &q, Natural &r, const Natural &num, const Natural &den)
        {
            q = Divide(num,den);
            r = num - den*q;
        }

        // num = q*den (+rem)
        Natural Natural:: Divide(const Natural &num, const Natural &den)
        {
            //__________________________________________________________________
            //
            //
            // get and check elements
            //
            //------------------------------------------------------------------
            Element &D = *den; if(D.bits<=0) throw Libc::Exception(EDOM, "%s division by zero", CallSign);
            Element &N = *num;

            switch( Element::Compare(N,D) )
            {
                case Negative: return Natural();               // N < D
                case __Zero__: return Natural(1);              // N == D
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
                    case __Zero__: return Natural(Exp2,p);
                    case Positive:
                        upper = new Natural(Exp2,p);   assert(num<*upper*den);
                        lower = new Natural(Exp2,--p); assert(*lower*den<num);
                        break;
                }
            }


            //__________________________________________________________________
            //
            //
            // bracket
            //
            //__________________________________________________________________
            Natural &lo = *lower;
            Natural &up = *upper;

            assert(lo*den<num);
            assert(num<up*den);

            const Natural one(1);
            while(true)
            {
                {
                    const Natural del = up-lo;
                    if(del<=one) return lo;
                }

                Natural        mid  = (lo+up).shr();
                const Natural probe = mid * den;

                switch( Compare(probe,num) )
                {
                    case Negative: lo.xch(mid); break;
                    case __Zero__: return mid;
                    case Positive: up.xch(mid); break;
                }

            }

        }



        Natural & Natural:: operator/=(const Natural &rhs)
        {
            Natural res = Divide(*this,rhs);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator/=(const uint64_t rhs)
        {
            const Natural den(rhs);
            return *this /= den;
        }


        Natural operator/(const Natural &lhs, const Natural &rhs)
        {
            return Natural::Divide(lhs,rhs);
        }

        Natural operator/(const uint64_t lhs, const Natural &rhs)
        {
            const Natural num(lhs);
            return Natural::Divide(num,rhs);
        }

        Natural operator/(const Natural &lhs, const uint64_t rhs)
        {
            const Natural den(rhs);
            return Natural::Divide(lhs,den);
        }

    }
}
