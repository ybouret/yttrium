#include "y/apex/natural.hpp"

#include "y/apex/block/factory.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Natural Natural:: Div(const Natural &num, const Natural &den)
        {

            const size_t denBits = den.bits(); if( denBits<=0 ) throw Libc::Exception(EDOM, "division by zero in Apex");
            const size_t numBits = num.bits();

            if(numBits<denBits) return Natural(0);

            assert(numBits>=denBits);
            size_t  p = numBits - denBits; // q = 2^p
            Natural a = den.shl(p);        // a = 2^p * den
            std::cerr << "a=" << a << " / " << num  << std::endl;
            switch( Compare(a,num) )
            {
                case __Zero__:
                    // a == num
                    return Natural(Exp2,p); // early return;

                case Positive:
                    // a > num
                    break;
            }

            return Natural(0);
        }

    }

}

