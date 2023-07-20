#include "y/apex/integer.hpp"
#include "y/io/chars.hpp"

namespace Yttrium
{
    namespace Apex
    {

        const char * const Integer :: CallSign = "Apex::Integer";

        Integer::  Integer() : s(__Zero__), n() {}
        
        Integer:: ~Integer() noexcept {}

        Integer:: Integer(const Integer &z) : s(z.s), n(z.n) {}

        Integer & Integer:: operator=(const Integer &other)
        {
            Integer tmp(other);
            xch(tmp);
            return *this;
        }


        void Integer:: xch(Integer &z) noexcept
        {
            Coerce(n).xch( Coerce(z.n) );
            CoerceSwap(s,z.s);
        }

        static uint64_t i2u(const SignType s, const int64_t i)
        {
            switch(s)
            {
                case Negative: return uint64_t(-i);
                case __Zero__: return 0;
                case Positive: return uint64_t(i);
            }
            return 0;
        }

        Integer:: Integer(const int64_t i) :
        s( SignOf(i) ),
        n( i2u(s,i)  )
        {
        }

        Integer & Integer:: operator=(const int64_t i)
        {
            switch( Coerce(s) = SignOf(i) )
            {
                case Negative: Coerce(n) = uint64_t(-i); break;
                case __Zero__: Coerce(n) = 0; break;
                case Positive: Coerce(n) = uint64_t(i);  break;
            }
            return *this;
        }

        void Integer:: appendTo(IO::Chars &cache) const
        {
            switch(s)
            {
                case Negative: cache << '-'; n.appendDec(cache); break;
                case __Zero__: cache << '0'; break;
                case Positive: n.appendDec(cache); break;
            }
        }

        std::ostream & operator<<(std::ostream &os, const Integer &z)
        {
            IO::Chars cache;
            z.appendTo(cache);
            Number::Display(os,cache);
            return os;
        }


    }
}
