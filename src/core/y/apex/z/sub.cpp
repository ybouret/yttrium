
#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {
        Integer Integer:: operator-() const
        {
            Integer _(*this);
            Sign::ReplaceByOpposite( Coerce(_.s) );
            return _;
        }




        void Integer:: decr()
        {
            switch(s)
            {
                case Negative: Coerce(n).incr(); break;

                case __Zero__:
                    Coerce(s) = Negative;
                    Coerce(n) = 1;
                    break;

                case Positive:
                    if(1==n->bits)
                    {
                        Coerce(s) = __Zero__;
                        Coerce(n) = 0;
                    }
                    else
                    {
                        Coerce(n).decr();
                    }
            }
        }

        Integer & Integer:: operator--()
        {
            decr();
            return *this;
        }

        Integer Integer:: operator--(int)
        {
            const Integer old(*this);
            decr();
            return old;
        }

    }

}


namespace Yttrium
{

    namespace Apex
    {

        Integer Integer:: Sub(const Integer &lhs, const Integer &rhs)
        {
            switch( Sign::MakePair(lhs.s,rhs.s) )
            {
                case NZ_Signs:
                case PZ_Signs:
                    assert(0==rhs);
                    return lhs;

                case ZN_Signs:
                case ZP_Signs:
                    assert(0==lhs);
                    return -rhs;

                    // add naturals with the sign of lhs
                case NP_Signs:
                case PN_Signs: { Integer _(lhs); Coerce(_.n) += rhs.n; return _; }

                case PP_Signs:
                    switch( Natural::Compare(lhs.n,rhs.n) )
                    {
                        case Negative: { const Natural delta = rhs.n - lhs.n; return Integer(Negative,delta); }
                        case Positive: { const Natural delta = lhs.n - rhs.n; return Integer(Positive,delta); }
                        case __Zero__: break; // => 0
                    }
                    break; // => 0


                case NN_Signs:
                    switch( Natural::Compare(lhs.n,rhs.n) )
                    {
                        case Negative: { const Natural delta = rhs.n - lhs.n; return Integer(Positive,delta); }
                        case Positive: { const Natural delta = lhs.n - rhs.n; return Integer(Negative,delta); }
                        case __Zero__: break; // => 0
                    }
                    break; // => 0

                case ZZ_Signs:
                    break; // => 0
            }

            return Integer(0);
        }

    }

}


namespace Yttrium
{

    namespace Apex
    {
        Integer Integer:: Sub(const Integer &lhs, const integer_t rhs)
        {
            return Add(lhs,-rhs);
        }

        Integer Integer:: Sub(const integer_t lhs, const Integer &rhs)
        {
            Integer _ = Add(-lhs,rhs);
            Sign::ReplaceByOpposite( Coerce(_.s) );
            return _;
        }

    }

}

namespace Yttrium
{

    namespace Apex
    {

        Integer Integer:: Sub(const Integer &lhs, const Natural &rhs)
        {
            if(rhs->bits<=0)
            {
                assert(0==rhs);
                return lhs;
            }
            else
            {
                assert(rhs>0);
                switch( lhs.s )
                {
                    case __Zero__: // 0 - rhs
                        return Integer(Negative,rhs);

                    case Negative: // - (|lhs|+rhs)
                    { Integer _(lhs); Coerce(_.n) += rhs; return _; }

                    case Positive:
                        switch( Natural::Compare(lhs.n,rhs) )
                        {
                            case __Zero__: break; // => 0
                            case Positive: { Integer _(lhs);          Coerce(_.n) -= rhs;   return _; }
                            case Negative: { Integer _(Negative,rhs); Coerce(_.n) -= lhs.n; return _; }
                        }
                        break; // => 0
                }
                return Integer(0);
            }
        }

        Integer Integer:: Sub(const Natural &lhs, const Integer &rhs)
        {
            Integer _ = Sub(rhs,lhs);
            Sign::ReplaceByOpposite( Coerce(_.s) );
            return _;
        }

    }

}
