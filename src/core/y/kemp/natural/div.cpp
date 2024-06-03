#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Kemp
    {

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
                case Negative: return Natural();
                case __Zero__: return Natural(1);
                case Positive:
                    assert(N.bits>=D.bits);
                    break;
            }


            //__________________________________________________________________
            //
            //
            // 2^p * den < num < 2^(p+1) * den
            //
            //__________________________________________________________________
            size_t           p     = N.bits-D.bits; std::cerr << "p=" << p << std::endl;
            Natural          probe = den << p;
            AutoPtr<Natural> lower = 0;
            AutoPtr<Natural> upper = 0;

            switch( Compare(probe,num) )
            {
                case Negative:
                    std::cerr << "negative" << std::endl;
                    lower = new Natural(probe); assert(*lower<num);
                    probe <<= 1;
                    upper = new Natural(probe); assert(num<*upper);
                    break;
                case __Zero__: return probe;
                case Positive:
                    std::cerr << "positive" << std::endl;
                    upper = new Natural(probe); assert(num<*upper);
                    probe.shr();
                    lower = new Natural(probe); assert(*lower<num);
                    break;
            }
            
            return Natural();
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
