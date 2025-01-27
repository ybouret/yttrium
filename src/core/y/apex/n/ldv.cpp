#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        void Natural:: Div(Natural &quot, Natural &rem, const Natural &num, const Natural &den)
        {
#if 1
            quot = Div(num,den);
            rem  = num-quot * den;
#else
            try
            {
                //--------------------------------------------------------------
                //
                // process den.bits()
                //
                //--------------------------------------------------------------
                const size_t denBits = den.bits();
                switch( denBits )
                {
                    case 0: throw Libc::Exception(EDOM, "%s Division By Zero",CallSign);
                    case 1:
                        assert(1==den);
                        quot = num;
                        rem  = 0;
                        return;

                    default:
                        break;
                }

                //--------------------------------------------------------------
                //
                // process num.bits()
                //
                //--------------------------------------------------------------
                const size_t numBits = num.bits();
                if(numBits<denBits)
                {
                    quot = 0;
                    rem  = num;
                    return;
                }

                //--------------------------------------------------------------
                //
                // bracket quotient by shift
                //
                //--------------------------------------------------------------
                size_t   shift = numBits-denBits;
                {
                    Natural  probe = den.shl(shift);
                    {
                    PROBE:
                        switch( Compare(probe,num) )
                        {
                            case __Zero__:
                                quot = Natural(Exp2,shift);
                                rem  = 0;
                                return;

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

                //--------------------------------------------------------------
                //
                // Bissection
                //
                //--------------------------------------------------------------
                Natural lower = Natural(Exp2,shift-1); assert(lower*den<num);
                {
                    Natural upper = Natural(Exp2,shift);   assert(upper*den>num);
                    do
                    {
                        Natural        middle = upper+lower; middle._shr();
                        {
                            const Natural  probe  = middle * den;
                            switch( Compare(probe,num) )
                            {
                                case Negative: lower.xch(middle); break;      // lower <- middle
                                case __Zero__: quot = middle; rem=0; return ; // exact value
                                case Positive: upper.xch(middle); break;      // upper <- middle
                            }
                        }
                        assert(lower<=upper);
                    }
                    while(upper-lower>1);
                }

                //--------------------------------------------------------------
                //
                // done!
                //
                //--------------------------------------------------------------
                quot.xch(lower);
                rem = num-quot*den;
            }
            catch(...)
            {
                quot.block->ldz();
                rem.block->ldz();
                throw;
            }
#endif
        }

    }

}

