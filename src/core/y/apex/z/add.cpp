
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
                    break; // => 0
            }
            return 0;
        }


        void Integer:: incr()
        {
            switch(s)
            {
                case Positive:
                    Coerce(n).incr();
                    break;

                case __Zero__:
                    Coerce(s) = Positive;
                    Coerce(n) = 1;
                    break;

                case Negative:
                    if(1==n->bits)
                    {
                        Coerce(s) = __Zero__;
                        Coerce(n) = 0;
                    }
                    else
                    {
                        Coerce(n).decr();
                    }
                    break;

            }
        }

        Integer & Integer:: operator++()
        {
            incr();
            return *this;
        }

        Integer Integer:: operator++(int)
        {
            const Integer old(*this);
            incr();
            return old;
        }
    }

}
