#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        // num = q*den (+rem)
        Natural Natural:: Divide(const Natural &num, const Natural &den)
        {
            Element &D = *den; if(D.bits<=0) throw Libc::Exception(EDOM, "%s division by zero", CallSign);
            Element &N = *num; if(N.bits<D.bits) { assert(num<den); return Natural(); }

            // 2^p * den <= num < 2^(p+1) * den
            
            return Natural();
        }



        Natural & Natural:: operator/=(const Natural &rhs)
        {
            Natural res = Divide(*this,rhs);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator*=(const uint64_t rhs)
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
