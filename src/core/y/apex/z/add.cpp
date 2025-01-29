
#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {
        Integer Integer:: operator+() const
        {
            return *this;
        }

        Integer Integer:: Add(const Integer &lhs, const Integer &rhs)
        {
            switch( Sign::MakePair(lhs.s,rhs.s) )
            {
                case ZN_Signs:
                case ZP_Signs: assert(0==lhs); return rhs;

                case NZ_Signs:
                case PZ_Signs: assert(0==rhs); return lhs;

                case PP_Signs: {
                    const Natural arg = lhs.n + rhs.n;
                    return Integer(Positive,arg);
                }

                case NN_Signs: {
                    const Natural arg = lhs.n + rhs.n;
                    return Integer(Negative,arg);
                }

                case PN_Signs:
                    assert(lhs>0);
                    assert(rhs<0);
                    switch( Natural::Compare(lhs.n,rhs.n) )
                    {
                        case Positive: {
                            assert(lhs.n>rhs.n);
                            const Natural arg = lhs.n - rhs.n;
                            return Integer(Positive,arg);
                        }

                        case Negative: {
                            assert(lhs.n<rhs.n);
                            const Natural arg = rhs.n - lhs.n;
                            return Integer(Negative,arg);
                        }

                        case __Zero__:
                            break; // => 0
                    }
                    break; // => 0

                case NP_Signs:
                    assert(lhs<0);
                    assert(rhs>0);
                    switch( Natural::Compare(lhs.n,rhs.n) )
                    {
                        case Positive: {
                            assert(lhs.n>rhs.n);
                            const Natural arg = lhs.n - rhs.n;
                            return Integer(Negative,arg);
                        }

                        case Negative: {
                            assert(lhs.n<rhs.n);
                            const Natural arg = rhs.n - lhs.n;
                            return Integer(Positive,arg);
                        }

                        case __Zero__:
                            break; // => 0
                    }
                    break; // => 0


                case ZZ_Signs:
                    break;
            }
            return 0;
        }
    }

}
