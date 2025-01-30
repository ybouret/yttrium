
#include "y/apex/integer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace Apex
    {
        Integer Integer:: Div(const Integer &lhs, const Integer &rhs)
        {
            switch( Sign::MakePair(lhs.s,rhs.s) )
            {
                case ZZ_Signs:
                case PZ_Signs:
                case NZ_Signs:
                    throw Specific::Exception(CallSign, "Division By Zero");

                case ZN_Signs:
                case ZP_Signs:
                    break; // => 0

                case NN_Signs:
                case PP_Signs:
                { const Natural q = lhs.n/rhs.n; return Integer(q); }

                case NP_Signs:
                case PN_Signs:
                { const Natural q = lhs.n/rhs.n; return -Integer(q); }
            }
            return Integer(0);
        }
    }

}
