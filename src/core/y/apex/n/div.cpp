#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Natural operator/(const Natural &lhs, const Natural &rhs)
        {
            return Natural::Div(lhs,rhs);
        }

        Natural operator/(const natural_t lhs, const Natural &rhs)
        {
            const Natural _(lhs);
            return Natural::Div(_,rhs);
        }


        Natural operator/(const Natural &lhs, const natural_t rhs)
        {
            const Natural _(rhs);
            return Natural::Div(lhs,_);
        }

        Natural & Natural:: operator/=(const Natural &rhs)
        {
            { Natural res = Div(*this,rhs); xch(res); }
            return *this;
        }

        Natural & Natural:: operator/=(const natural_t rhs)
        {
            { const Natural _(rhs); Natural res = Div(*this,_); xch(res); }
            return *this;
        }



        Natural Natural:: Div(const Natural &num, const Natural &den)
        {
            //------------------------------------------------------------------
            //
            // process den.bits()
            //
            //------------------------------------------------------------------
            const size_t denBits = den.bits();
            switch( denBits )
            {
                case 0: // error
                    throw Libc::Exception(EDOM, "%s Division By Zero",CallSign);

                case 1: // unit denominator
                    assert(1==den);
                    return num;

                default: // generic case
                    assert(den>1);
                    break;
            }


            switch( Compare(num,den) )
            {
                case Negative: return 0; // early return
                case __Zero__: return 1; // early return
                case Positive:
                    break;
            }

            //------------------------------------------------------------------
            //
            // bracket quotient by shift
            //
            //------------------------------------------------------------------
            assert(num>den);              // num > 2^0 * den
            const size_t numBits = num.bits(); assert(numBits>=denBits);
            size_t       shift   = Max<size_t>(numBits-denBits,1);
            {
                Natural  probe = den.shl(shift);
                {
                PROBE:
                    switch( Compare(probe,num) )
                    {
                        case __Zero__:
                            // specific case
                            return Natural(Exp2,shift);

                        case Negative:
                            ++shift;
                            probe <<= 1;
                            goto PROBE;

                        case Positive:
                            break;
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
                        case __Zero__: return middle;            // exact value
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
            return lower;
        }

    }

}

