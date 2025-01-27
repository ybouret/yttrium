#include "y/apex/natural.hpp"

#include "y/apex/block/factory.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        static inline
        Natural DivAlgo(const Natural &num,
                        const Natural &den,
                        const size_t   lowerShift)
        {
            assert( den.shl(lowerShift) < num );

            return 0;
        }


        
        Natural Natural:: Div(const Natural &num, const Natural &den)
        {

            const size_t denBits = den.bits();
            switch( denBits )
            {
                case 0: throw Libc::Exception(EDOM, "%s Division By Zero",CallSign);
                case 1: assert(1==den); return num;
                default:
                    break;
            }

            const size_t numBits = num.bits();

            switch( Sign::Of(numBits,denBits) )
            {
                case Negative:
                    assert(numBits<denBits);
                    assert(num<den);
                    return 0;

                case Positive:
                    assert(numBits>denBits);
                    assert(num>den);
                    assert( den.shl(numBits-denBits-1) < num );
                    return DivAlgo(num,den,numBits-denBits-1);

                case __Zero__:
                    break;
            }


            return Natural(0);
        }

    }

}

