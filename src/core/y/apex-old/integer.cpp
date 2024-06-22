#include "y/apex/integer.hpp"
#include "y/stream/io/chars.hpp"
#include "y/random/bits.hpp"
#include "y/system/exception.hpp"

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

        Integer:: Integer(const SignType _s, const Natural _n) : s(_s), n(_n)
        {
            assert(!( (__Zero__==s) && (n>0) ) );
        }

        Integer:: Integer(const size_t numBits, Random::Bits &ran) :
        s( numBits <= 0 ? __Zero__ : ( ran.choice() ? Positive:Negative) ),
        n(numBits,ran)
        {

        }

        void Integer:: ldz() noexcept
        {
            Coerce(s) = __Zero__;
            Coerce(n).ldz();
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
        s( Sign::Of(i) ),
        n( i2u(s,i)  )
        {
        }

        Integer & Integer:: operator=(const int64_t i)
        {
            switch( Coerce(s) = Sign::Of(i) )
            {
                case Negative: Coerce(n) = uint64_t(-i); break;
                case __Zero__: Coerce(n) = 0; break;
                case Positive: Coerce(n) = uint64_t(i);  break;
            }
            return *this;
        }

        Integer:: Integer(const Natural &N) :
        s( N.getSign() ),
        n( N )
        {
        }

        Integer & Integer:: operator=(const Natural &N)
        {
            Integer tmp(N);
            xch(tmp);
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

        Integer Integer:: incr() const
        {
            switch(s)
            {
                case Negative:
                    if(n==1)
                    {
                        return Integer();
                    }
                    else
                    {
                        Natural p=n; assert(p>1);
                        return Integer(Negative,--p);
                    }

                case Positive: {
                    Natural p = n; assert(p>0);
                    return Integer(Positive,++p);
                }

                case __Zero__:
                    break;
            }
            return Integer(1);
        }


        Integer Integer:: decr() const
        {
            switch(s)
            {
                case Negative: {
                    Natural p = n; assert(p>0);
                    return Integer(Negative,++p);
                }

                case Positive: {
                    if(n==1)
                    {
                        return Integer();
                    }
                    else
                    {
                        Natural p = n; assert(p>1);
                        return Integer(Positive,--p);
                    }
                }

                case __Zero__:
                    break;
            }
            return Integer(-1);
        }

        void Integer:: ThrowOverflow(const char *which)
        {
            if(which)
                throw Specific::Exception(CallSign,"during cast to '%s'", which);
            else
                throw Specific::Exception(CallSign,"during cast");
        }

    }
}
