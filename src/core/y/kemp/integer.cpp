
#include "y/kemp/integer.hpp"
#include "y/random/bits.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        const char * const Integer:: CallSign = "apz";

        void Integer:: CastOverflow(const char *ctx)
        {
            throw Specific::Exception(CallSign, "cast overflow for '%s'", ctx ? ctx : Core::Unknown);
        }


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

        Integer   & Integer:: neg() noexcept
        {
            Sign::ReplaceByOpposite( Coerce(s) );
            return *this;
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

        Integer:: Integer(const size_t nbits, Random::Bits &ran) :
        Number(),
        s( (nbits <= 0) ? __Zero__ : ( ran.choice() ? Negative : Positive) ),
        n(nbits,ran)
        {
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

        String Integer:: toString() const
        {
            if(Negative==s) 
                return '-' + n.toDec();
            else
                return n.toDec();
        }

        std::ostream & operator<<(std::ostream &os, const Integer &z)
        {
            os << z.toString();
            return os;
        }
        
        const char * Integer:: callSign() const noexcept { return CallSign; }

    }
}
