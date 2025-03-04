#include "y/kemp/integer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        void Integer::DivisionByZero()
        {
            throw Specific::Exception(CallSign, "Division By Zero");
        }

        Integer Integer:: Div(const Integer &lhs, const Integer &rhs)
        {
            return Div(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        Integer Integer:: Div(const Integer &lhs, const Natural &rhs)
        {
            return Div(lhs.s,lhs.n,rhs.sign(),rhs);
        }

        Integer Integer:: Div(const Natural &lhs, const Integer &rhs)
        {
            return Div(lhs.sign(),lhs,rhs.s,rhs.n);
        }


        Integer Integer:: Div(const Integer &lhs, const int64_t rhs)
        {
            switch( Sign::Of(rhs) )
            {
                case __Zero__: break;
                case Positive: { const uint64_t u = static_cast<uint64_t>( rhs); return Div(lhs.s,lhs.n,Positive,u); }
                case Negative: { const uint64_t u = static_cast<uint64_t>(-rhs); return Div(lhs.s,lhs.n,Negative,u); };
            }
            DivisionByZero();
            return Integer();
        }

        Integer Integer:: Div(const int64_t lhs, const Integer &rhs)
        {
            switch( Sign::Of(lhs) )
            {
                case __Zero__: break;
                case Positive: { const uint64_t u = static_cast<uint64_t>( lhs);  return Div(Positive,u,rhs.s,rhs.n); }
                case Negative: { const uint64_t u = static_cast<uint64_t>(-lhs);  return Div(Negative,u,rhs.s,rhs.n); }
            }
            return Integer();
        }
    }

}
