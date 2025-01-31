
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
                    throw Specific::Exception(CallSign, "Integer/Integer Division By Zero");

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

namespace Yttrium
{

    namespace Apex
    {

        Integer Integer:: Div(const Integer &lhs, const Natural &rhs)
        {
            switch(rhs->bits)
            {
                case 0: throw Specific::Exception(CallSign, "Integer/Natural Division By Zero");
                case 1: assert(1==rhs); return lhs;
                default:
                    break;
            }
            switch(lhs.s)
            {
                case __Zero__: break; // => 0
                case Positive: { const Natural q = lhs.n/rhs; return  Integer(q); }
                case Negative: { const Natural q = lhs.n/rhs; return -Integer(q); }
            }
            return Integer(0);
        }

        Integer Integer:: Div(const Natural &lhs, const Integer &rhs)
        {
            bool takeOpposite = false;
            switch(rhs.s)
            {
                case __Zero__: throw Specific::Exception(CallSign, "Natural/Integer Division By Zero");
                case Positive: break;
                case Negative: takeOpposite = true; break;
            }
            const Natural q = lhs/rhs.n;
            return takeOpposite ? -Integer(q) : Integer(q);
        }

    }

}

namespace Yttrium
{

    namespace Apex
    {
        static inline Integer MakeDiv(const bool      takeOpposite,
                                      const Integer & lhs,
                                      const natural_t rhs)
        {
            assert(0!=rhs);
            const Natural q = lhs.n/rhs;
            return takeOpposite ? -Integer(q) : Integer(q);
        }

        Integer Integer:: Div(const Integer &lhs, const integer_t rhs)
        {

            switch( Sign::Of(rhs) )
            {
                case __Zero__: break; // => throw
                case Positive: return MakeDiv(Negative == lhs.s, lhs, static_cast<natural_t>( rhs) );
                case Negative: return MakeDiv(Positive == lhs.s, lhs, static_cast<natural_t>(-rhs) );

            }
            throw Specific::Exception(CallSign, "Integer/integer_t Division By Zero");
        }
    }

}

