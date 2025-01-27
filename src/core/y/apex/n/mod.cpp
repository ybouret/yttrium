
#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Natural operator%(const Natural &lhs, const Natural &rhs)
        {
            return Natural::Mod(lhs,rhs);
        }

        Natural operator%(const natural_t lhs, const Natural &rhs)
        {
            const Natural _(lhs);
            return Natural::Mod(_,rhs);
        }


        Natural operator%(const Natural &lhs, const natural_t rhs)
        {
            const Natural _(rhs);
            return Natural::Mod(lhs,_);
        }

        Natural & Natural:: operator%=(const Natural &rhs)
        {
            { Natural res = Mod(*this,rhs); xch(res); }
            return *this;
        }

        Natural & Natural:: operator%=(const natural_t rhs)
        {
            { const Natural _(rhs); Natural res = Mod(*this,_); xch(res); }
            return *this;
        }


        Natural Natural:: Mod(const Natural &num, const Natural &den)
        {
            //------------------------------------------------------------------
            //
            // process den.bits()
            //
            //------------------------------------------------------------------
            const size_t denBits = den.bits();
            switch( denBits )
            {
                case 0: throw Libc::Exception(EDOM, "%s Division By Zero",CallSign);
                case 1: assert(1==den); return 0;
                default:
                    break;
            }

            //------------------------------------------------------------------
            //
            // process num.bits()
            //
            //------------------------------------------------------------------
            const size_t numBits = num.bits();
            if(numBits<denBits) return num;

            //------------------------------------------------------------------
            //
            // bracket quotient by shift
            //
            //------------------------------------------------------------------
            size_t   shift = numBits-denBits;
            {
                Natural  probe = den.shl(shift);
                {
                PROBE:
                    switch( Compare(probe,num) )
                    {
                        case __Zero__: return 0; // exact multiple
                        case Negative:
                            ++shift;
                            probe <<= 1;
                            goto PROBE;
                        case Positive: break;
                    }
                }
                assert(probe>num);
                assert(shift>0);
            }

            //------------------------------------------------------------------
            //
            // Bissection
            //
            //------------------------------------------------------------------
            Natural upper = Natural(Exp2,shift);   assert(upper*den>num);
            Natural lower = Natural(Exp2,shift-1); assert(lower*den<num);
            do
            {
                Natural        middle = upper+lower; middle._shr();
                {
                    const Natural  probe  = middle * den;
                    switch( Compare(probe,num) )
                    {
                        case Negative: lower.xch(middle); break; // lower <- middle
                        case __Zero__: return 0;                 // exact value
                        case Positive: upper.xch(middle); break; // upper <- middle
                    }
                }
                assert(lower<=upper);
            }
            while(upper-lower>1);

            //------------------------------------------------------------------
            //
            // done!
            //
            //------------------------------------------------------------------
            return num - lower*den;
        }

    }

}

