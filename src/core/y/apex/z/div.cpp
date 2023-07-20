
#include "y/apex/integer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Integer operator/(const Integer &lhs, const Integer &rhs)
        {
            SignType S = __Zero__;
            switch( Sign::MakePair(lhs.s,rhs.s) )
            {
                case NZ_Signs:
                case ZZ_Signs:
                case PZ_Signs:
                    throw Specific::Exception(Integer::CallSign,"Division By Zero");

                case ZP_Signs:
                case ZN_Signs:
                    return Integer();

                case NN_Signs:
                case PP_Signs:
                    S = Positive;
                    break;

                case NP_Signs:
                case PN_Signs:
                    S = Negative;
                    break;
            }
            assert(lhs.n>0);
            assert(rhs.n>0);
            const Natural N = lhs.n/rhs.n;
            return Integer(S,N);

        }


        Integer operator/(const Integer &lhs, const int64_t rhs)
        {
            const Integer tmp(rhs);
            return lhs / tmp;
        }

        Integer operator/(const int64_t lhs, const Integer &rhs)
        {
            const Integer tmp(lhs);
            return tmp / rhs;
        }

        Integer operator/(const Integer &lhs, const Natural &rhs)
        {
            const Integer tmp(rhs);
            return lhs/tmp;
        }

        Integer operator/(const Natural &lhs, const Integer &rhs)
        {
            const Integer tmp(lhs);
            return tmp/rhs;
        }


        Integer & Integer :: operator/=(const Integer &rhs)
        {
            Integer tmp = (*this) / rhs;
            xch(tmp);
            return *this;
        }

        Integer & Integer :: operator/=(const int64_t rhs)
        {
            Integer tmp = (*this) / rhs;
            xch(tmp);
            return *this;
        }

        Integer & Integer :: operator/=(const Natural &rhs)
        {
            Integer tmp = (*this) / rhs;
            xch(tmp);
            return *this;
        }
    }

}

