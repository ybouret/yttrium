#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        SignType Integer:: Cmp(const SignType  ls,
                              const Natural  &ln,
                              const SignType  rs,
                              const Natural  &rn) noexcept
        {
            switch( Sign::MakePair(ls,rs) )
            {

                    

                case ZZ_Signs:
                    assert(0==ln);assert(0==rn);
                    break;
            }
            return __Zero__;
        }

    }
}
