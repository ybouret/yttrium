
#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        Integer::Integer() : Number(), s(__Zero__), n()
        {
        }

        Integer:: ~Integer() noexcept
        {
            Coerce(s) = __Zero__;
        }


        static inline uint64_t i2u(const int64_t i) noexcept
        {
            return i < 0 ? static_cast<uint64_t>(-i) : static_cast<uint64_t>(i);
        }

        Integer:: Integer(const int64_t z) :
        Number(),
        s( Sign::Of(z) ),
        n( i2u(z) )
        {

        }

        Integer:: Integer(const Integer &z) : Number(), s(z.s), n(z.n)
        {
        }

        Integer:: Integer(const SignType S, const Natural &N) :
        Number(),
        s(S),
        n(N)
        {
            if(n<=0) Coerce(S) = __Zero__;
        }

        Integer & Integer:: operator=(const Integer &z)
        {
            Coerce(s) = z.s;
            Coerce(n) = z.n;
            return *this;
        }

        Integer & Integer:: operator=(const int64_t  z)
        {
            Integer _(z);
            xch(_);
            return *this;
        }

        void Integer:: xch(Integer &z) noexcept
        {
            CoerceSwap(s,z.s);
            Coerce(n).xch( Coerce(z.n) );
        }


        std::ostream & operator<<(std::ostream &os, const Integer &z)
        {
            switch(z.s)
            {
                case Negative: assert(z.n>0);  os << '-' << z.n; break;
                case __Zero__: assert(0==z.n); os << z.n; break;
                case Positive: assert(z.n>0);  os << z.n; break;
            }
            return os;
        }


    }
}
