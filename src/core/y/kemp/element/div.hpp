
#ifndef Y_Kemp_Element_Div_Included
#define Y_Kemp_Element_Div_Included 1

#include "y/kemp/element.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        struct Division
        {

            // return q such that num = q*den (+ rem)
            static
            Element * Run(Element &num,
                          Element &den)
            {

                // get rid of trivial cases
                if(den.bits<=0)       throw Libc::Exception(EDOM,"%s division by zero", Element::CallSign);
                if(num.bits<den.bits) return new Element(0,AsCapacity);

                assert(num.bits>=den.bits);

                // look for p such that
                // 2^p * den <= num < 2^(p+1) * num
                size_t p = num.bits-den.bits;



                return 0;
            }


        };
    }

}

#endif
