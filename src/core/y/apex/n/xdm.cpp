
#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Apex
    {
        namespace
        {
            struct XDM
            {
                enum Return
                {
                    Div,
                    Mod,
                    Both
                };

                static inline Return Get(const Natural:: DivModArg * const arg) noexcept
                {
                    if(0==arg) {
                        return Div;
                    }
                    else
                    {
                        if(0==arg->rem)
                        {
                            return Mod;
                        }
                        else
                        {
                            return Both;
                        }
                    }
                }
            };

        }


#define XDM_RETURN(QUOT,REM)                                       \
/**/    switch(ret) {                                              \
/**/        case XDM::Mod: return (REM);                           \
/**/        case XDM::Both: assert(arg->rem); *(arg->rem) = (REM); \
/**/        case XDM::Div:  break; }                               \
/**/    return (QUOT)

        Natural Natural:: DivMod(const Natural    &num,
                                 const Natural    &den,
                                 DivModArg * const arg)
        {
            //------------------------------------------------------------------
            //
            // get return type
            //
            //------------------------------------------------------------------
            const XDM::Return ret = XDM::Get(arg);

            //------------------------------------------------------------------
            //
            // process den.bits()
            //
            //------------------------------------------------------------------
            const size_t denBits = den->bits;
            switch( denBits )
            {
                case 0: // error
                    throw Libc::Exception(EDOM, "%s Division By Zero",CallSign);

                case 1: // unit denominator => quotient=num, remainder=0
                    assert(1==den);
                    XDM_RETURN(num,0);

                default: // generic case
                    assert(den>1);
                    break;
            }

            //------------------------------------------------------------------
            //
            // process comparison
            //
            //------------------------------------------------------------------
            switch( Compare(num,den) )
            {
                case Negative: // early return, quotient=0, remainder=num
                    XDM_RETURN(0,num);

                case __Zero__: // early return, quotient=1, remainder=0
                    XDM_RETURN(1,0);

                case Positive: // default algorithm
                    break;
            }

            //------------------------------------------------------------------
            //
            // bracket quotient by shift > 0
            //
            //------------------------------------------------------------------
            assert(num>den); // implies num > 2^0 * den
            size_t       shift = Max<size_t>(num->bits-denBits,1);
            {
                Natural  probe = den.shl(shift);
                {
                PROBE:
                    switch( Compare(probe,num) )
                    {
                        case __Zero__: // early return, quotient = 2^shift, remainder=0
                            XDM_RETURN(Natural(Exp2,shift),0);

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
            Natural     lower = Natural(Exp2,shift-1); assert(lower*den<num);
            {
                Natural upper = Natural(Exp2,shift);   assert(upper*den>num);
                do
                {
                    Natural        middle = upper+lower; middle._shr();
                    {
                        const Natural  probe  = middle * den;
                        switch( Compare(probe,num) )
                        {
                            case Negative: lower.xch(middle); break; // lower <- middle
                            case __Zero__: XDM_RETURN(middle,0);     // early return, quotient=middle, remain=0
                            case Positive: upper.xch(middle); break; // upper <- middle
                        }
                    }
                    assert(lower<=upper);
                }
                while( (upper-lower)->bits > 1);
            }

            //------------------------------------------------------------------
            //
            // done!
            //
            //------------------------------------------------------------------

            // quotient=lower, remain=num-lower*den
            XDM_RETURN(lower,num-lower*den);
        }
    }

}
