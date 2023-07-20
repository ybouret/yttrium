#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Integer operator*(const Integer &lhs, const Integer &rhs)  
        {
            SignType S = __Zero__;
            switch( Sign::MakePair(lhs.s, rhs.s) )
            {
                    // results in 0
                case NZ_Signs:
                case ZN_Signs:
                case ZZ_Signs:
                case ZP_Signs:
                case PZ_Signs:
                    return Integer();

                    // results in > 0
                case NN_Signs:
                case PP_Signs:
                    S = Positive;
                    break;

                    // results in < 0
                case NP_Signs:
                case PN_Signs:
                    S = Negative;
                    break;
            }
            assert(lhs.n>0);
            assert(rhs.n>0);
            const Natural P = lhs.n * rhs.n;
            return Integer(S,P);
        }


        Integer operator*(const Integer &lhs, const int64_t rhs)
        {
            const Integer tmp(rhs);
            return lhs * tmp;
        }

        Integer operator*(const int64_t lhs, const Integer &rhs)
        {
            const Integer tmp(lhs);
            return tmp * rhs;
        }

        Integer operator*(const Integer &lhs, const Natural &rhs)
        {
            SignType S = __Zero__;
            switch( Sign::MakePair(lhs.s, rhs.getSign()) )
            {
                    // results in 0
                case NZ_Signs:
                case ZN_Signs:
                case ZZ_Signs:
                case ZP_Signs:
                case PZ_Signs:
                    return Integer();

                    // results in > 0
                case NN_Signs:
                case PP_Signs:
                    S = Positive;
                    break;

                    // results in < 0
                case NP_Signs:
                case PN_Signs:
                    S = Negative;
                    break;
            }
            assert(lhs.n>0);
            assert(rhs>0);
            const Natural P = lhs.n * rhs;
            return Integer(S,P);
        }

        Integer operator*(const Natural &lhs, const Integer &rhs)
        {
            return rhs * lhs;
        }
    }

}

