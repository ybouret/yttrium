
#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {

        Integer Integer:: Mul(const Integer &lhs, const Integer &rhs)
        {
            switch( Sign::MakePair(lhs.s,rhs.s) )
            {
                case ZN_Signs:
                case ZP_Signs:
                case ZZ_Signs:
                case PZ_Signs:
                case NZ_Signs:
                    break;

                case NN_Signs:
                case PP_Signs:
                { Integer _(Positive,lhs.n); Coerce(_.n) *= rhs.n; return _; }

                case NP_Signs:
                case PN_Signs:
                { Integer _(Negative,lhs.n); Coerce(_.n) *= rhs.n; return _; }

            }

            return Integer(0);
        }

    }

}
