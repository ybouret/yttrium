
#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        const char * const Integer:: CallSign = "apz";

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

        Integer:: Integer(const Natural &N) : Number(),
        s( N.sign() ),
        n( N )
        {
        }

        Integer:: Integer(const Integer &z) : Number(), s(z.s), n(z.n)
        {
        }

        void Integer:: chk() noexcept
        {
            if(__Zero__==n.sign()) Coerce(s) = __Zero__;
        }

        Integer:: Integer(const SignType S, const Natural &N) :
        Number(),
        s(S),
        n(N)
        {
            chk();
        }

        Integer:: Integer(const SignType S, const uint64_t N) :
        Number(),
        s(S),
        n(N)
        {
            chk();
        }

        Integer & Integer:: operator=(const Integer &z) {
            Integer _(z); xch(_); return *this;
        }

        Integer & Integer:: operator=(const int64_t  z) {
            Integer _(z); xch(_); return *this;
        }

        Integer & Integer:: operator=(const Natural &N) {
            Integer _(N); xch(_); return *this;
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
