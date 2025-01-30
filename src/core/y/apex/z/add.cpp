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


namespace Yttrium
{

    namespace Apex
    {
        Integer Integer:: Add(const Integer &lhs, const Natural &rhs)
        {

            if(rhs->bits<=0)
            {
                return lhs;
            }
            else
            {
                assert(rhs>0);
                switch(lhs.s)
                {
                    case Positive: { Integer _(lhs); Coerce(_.n) += rhs; return _; }
                    case __Zero__: return Integer(Positive,rhs);
                    case Negative:
                        break;
                }
                assert(lhs<0);
                switch( Natural::Compare(lhs.n,rhs) )
                {
                    case Negative: // |lhs| < n
                    {
                        const Natural delta = rhs - lhs.n; assert(delta>0);
                        return Integer(Positive,delta);
                    }
                    case Positive: // |lhs| > n
                    {
                        const Natural delta = lhs.n - rhs;
                        return Integer(Negative,delta);
                    }
                    case __Zero__: break; // =>0
                }
                return Integer(0);
            }

        }

        Integer Integer:: Add(const Natural &lhs, const Integer &rhs)
        {
            return Add(rhs,lhs);
        }
    }

}

namespace Yttrium
{

    namespace Apex
    {
        Integer Integer:: Add(const Integer &lhs, const integer_t rhs)
        {
            switch( Sign::MakePair(lhs.s, Sign::Of(rhs ) ) )
            {
                case ZN_Signs:
                case ZP_Signs:
                    assert(0==lhs);
                    assert(0!=rhs);
                    return Integer(rhs);

                case NZ_Signs:
                case PZ_Signs:
                    assert(0!=lhs);
                    assert(0==rhs);
                    return lhs;

                case PP_Signs: { Integer _(lhs); Coerce(_.n) += static_cast<natural_t>( rhs); return _; }
                case NN_Signs: { Integer _(lhs); Coerce(_.n) += static_cast<natural_t>(-rhs); return _; }

                case PN_Signs: {
                    assert(lhs>0);
                    assert(rhs<0);
                    const Natural & l = lhs.n;
                    const natural_t n = static_cast<natural_t>(-rhs);
                    switch( Natural::Compare(l,n) )
                    {
                        case Negative: { // lhs < |rhs| => Negative
                            const Natural delta = n - l;
                            return Integer(Negative,delta);
                        }

                        case __Zero__:
                            break; // lhs = |rhs| => zero

                        case Positive: { // lhs > |rhs| => positive
                            const Natural delta = l - n;
                            return Integer(Positive,delta);
                        }
                    }
                } break; // => 0

                case NP_Signs: {
                    assert(lhs<0);
                    assert(rhs>0);
                    const Natural & l = lhs.n;
                    const natural_t n = static_cast<natural_t>(rhs);
                    switch( Natural::Compare(l,n) )
                    {
                        case Negative: { // lhs < |rhs| => Positive
                            const Natural delta = n - l;
                            return Integer(Positive,delta);
                        }

                        case __Zero__:
                            break; // lhs = |rhs| => zero

                        case Positive: { // lhs > |rhs| => Negative
                            const Natural delta = l - n;
                            return Integer(Negative,delta);
                        }
                    }
                } break; // => 0


                case ZZ_Signs:
                    break; // => 0
            }
            return Integer(0);
        }

        Integer Integer:: Add(const integer_t lhs, const Integer &rhs)
        {
            return Add(rhs,lhs);
        }
    }

}
