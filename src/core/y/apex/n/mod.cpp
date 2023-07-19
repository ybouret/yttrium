
#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Apex
    {
        //----------------------------------------------------------------------
        //
        //
        // Modulo
        //
        //
        //----------------------------------------------------------------------
        Natural operator%(const Natural &numer, const Natural &denom)
        {
            return numer-(numer/denom)*denom;

#if 0
            //------------------------------------------------------------------
            //
            // get proto Denom and Numer
            //
            //------------------------------------------------------------------
            const Prototype &D = CONST_PROTO(denom); if(D.nbits<=0) throw Specific::Exception(Natural::CallSign,"Division by Zero");
            const Prototype &N = CONST_PROTO(numer);

            //------------------------------------------------------------------
            //
            // check relative values
            //
            //------------------------------------------------------------------
            switch( Prototype::Compare(N,D) )
            {
                case Negative: return Natural(0);
                case __Zero__: return Natural(1);
                case Positive:
                    break;
            }
            assert(N.nbits>=D.nbits);
            assert(numer>denom);

            //------------------------------------------------------------------
            //
            // find denom * 2^(p-1) < numer < denom * 2^p
            //
            //------------------------------------------------------------------
            size_t p = Max<size_t>(N.nbits-D.nbits,1);
        PROBE:
            Natural upper(TwoToThe,p);
            {
                const Natural probe = upper * denom;
                switch( Natural::Compare(probe,numer) )
                {
                    case Negative: ++p; goto PROBE;
                    case __Zero__: return upper;
                    case Positive: break;
                }
            }
            Natural lower(TwoToThe,--p);
            assert(lower*denom<numer);
            assert(upper*denom>numer);

            //------------------------------------------------------------------
            //
            // Bisection
            //
            //------------------------------------------------------------------
            while(true)
            {
                Natural mid = (lower+upper); PROTO(mid).shr();
                {
                    const Natural probe = mid * denom;
                    switch( Natural::Compare(probe,numer) )
                    {
                        case Negative: mid.xch(lower); break;
                        case Positive: mid.xch(upper); break;
                        case __Zero__: return mid;
                    }
                }
                const AutoProto limit( Prototype::Add1( PROTO(lower)) );
                if( Negative != Prototype::Compare(*limit,CONST_PROTO(upper)))
                    return lower;

            }
#endif
        }


        Natural operator%(const Natural &lhs, const uint64_t rhs)
        {
            const Natural Denom(rhs);
            return lhs%Denom;
        }

        Natural operator%(const uint64_t lhs, const Natural &rhs)
        {
            const Natural Numer(lhs);
            return Numer%rhs;
        }

        Natural & Natural :: operator %=(const Natural &rhs)
        {
            Natural q = *this / rhs;
            xch(q);
            return *this;
        }

        Natural & Natural :: operator %=(const uint64_t rhs)
        {
            Natural q = *this / rhs;
            xch(q);
            return *this;
        }

    }

}


