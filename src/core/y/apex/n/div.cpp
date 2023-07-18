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
        // Division
        //
        //
        //----------------------------------------------------------------------
        Natural operator/(const Natural &numer, const Natural &denom)
        {
            const Prototype &D = CONST_PROTO(denom.impl); if(D.nbits<=0) throw Specific::Exception(Natural::CallSign,"Division by Zero");
            const Prototype &N = CONST_PROTO(numer.impl);
            switch( Prototype::Compare(N,D) )
            {
                case Negative: return Natural(0);
                case __Zero__: return Natural(1);
                case Positive:
                    break;
            }
            assert(N.nbits>=D.nbits);
            assert(numer>denom);

            size_t p = Max<size_t>(N.nbits-D.nbits,1);
        FIND_UPPER:
            Natural upper(TwoToThe,p);
            {
                Natural probe = upper * denom;
                switch( Natural::Compare(probe,numer) )
                {
                    case Negative: ++p; goto FIND_UPPER;
                    case __Zero__: return upper;
                    case Positive: break;
                }
            }
            Natural lower(TwoToThe,--p);
            assert(lower*denom<numer);
            assert(upper*denom>numer);
            //std::cerr << "quotient between 2^" << p << " and 2^" << p+1 << std::endl;

            while(true)
            {
                Natural mid = (lower+upper);
                PROTO(mid.impl).shr();
                //std::cerr << "testing " << mid << " in " << lower << " : " << upper << std::endl;
                {
                    const Natural probe = mid * denom;
                    switch( Natural::Compare(probe,numer) )
                    {
                        case Negative: mid.xch(lower); break;
                        case Positive: mid.xch(upper); break;
                        case __Zero__: return mid;
                    }
                }
                Natural limit = lower;
                if(++limit>=upper)
                    return lower;
            }



        }
    }

}


