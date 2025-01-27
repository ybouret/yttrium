#include "y/apex/natural.hpp"

#include "y/apex/block/factory.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {



        
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

            if(numBits<denBits) return 0;

            assert(numBits>=denBits);
            size_t   shift = numBits-denBits;
            Natural  probe = den.shl(shift);

            {
            PROBE:
                switch( Compare(probe,num) )
                {
                    case __Zero__: return Natural(Exp2,shift);
                    case Negative:
                        ++shift;
                        probe <<= 1;
                        goto PROBE;
                    case Positive: break;
                }
            }

            //std::cerr << "num=" << num.lsw() << " / den=" << den.lsw() << " : probe=" << probe.lsw() << std::endl;

            return Natural(0);
        }

    }

}

