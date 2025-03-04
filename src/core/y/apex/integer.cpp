
#include "y/apex/integer.hpp"
#include "y/system/exception.hpp"
#include <cerrno>

namespace Yttrium
{

    namespace Apex
    {

        const char * const Integer:: CallSign = "Apex::Integer";
        
        const char * Integer:: callSign() const noexcept
        {
            return CallSign;
        }

        static inline
        natural_t z2n(const SignType  s,
                      const integer_t z) noexcept
        {
            switch(s)
            {
                case Negative: return -z;
                case Positive: return  z;
                case __Zero__: break;
            }
            assert(0==z);
            return 0;
        }

        Integer:: Integer() :
        Castable(),
        s(__Zero__),
        n()
        {
        }
        

        Integer:: Integer(const integer_t z) :
        Castable(),
        s( Sign::Of(z) ),
        n( z2n(s,z) )
        {
        }

        Integer:: ~Integer() noexcept
        {
        }

        Integer:: Integer(const Integer &z) :
        Castable(),
        s(z.s),
        n(z.n)
        {
        }

        Integer:: Integer(const Natural &u) :
        Castable(),
        s( u._sgn() ),
        n( u )
        {
        }
        
        Integer:: Integer(const SignType theSign, const Natural &u) :
        Castable(),
        s( theSign ),
        n( u )
        {
            switch( s )
            {
                case __Zero__:
                    if(n>0)  throw Specific::Exception(CallSign,"invalid positive absolute value");
                    break;

                case Negative:
                case Positive:
                    if(n==0) throw Specific:: Exception(CallSign,"invalid null absolute value");
                    break;
            }
        }

        Integer & Integer:: xch( Integer &_ ) noexcept
        {
            CoerceSwap(_.s,s);
            Coerce(_.n).xch( Coerce(n) );
            return *this;
        }


        Integer & Integer:: operator=(const Integer &z)
        {
            Integer _(z);
            return xch(_);
        }

        Integer & Integer:: operator=(const integer_t z) noexcept
        {
            switch( Coerce(s) = Sign::Of(z) )
            {
                case __Zero__: Coerce(n) = 0; break;
                case Positive: Coerce(n) = static_cast<natural_t>(z); break;
                case Negative: Coerce(n) = static_cast<natural_t>(-z); break;
            }
            return*this;
        }

        void Integer:: ldz() noexcept
        {
            Coerce(n).ldz();
            Coerce(s) = __Zero__;
        }

        Integer & Integer:: operator=(const Natural &u)
        {
            Integer _(u);
            return xch(_);
        }


        Integer:: Integer(Random::Bits &ran, const size_t nbit) :
        s( __Zero__ ),
        n(ran,nbit)
        {
            if( Positive == (Coerce(s) = n._sgn()) )
            {
                if( ran.choice() ) Coerce(s) = Negative;
            }

        }

        String Integer:: toDec() const
        {
            switch( s )
            {
                case __Zero__: break;
                case Positive: return n.toDec();
                case Negative: return '-' + n.toDec();
            }
            return '0';
        }

        std::ostream & operator<<(std::ostream &os, const Integer &z)
        {
            return os << z.toDec();
        }


        Integer Integer:: sqr() const
        {
            return n.sqr();
        }

        Integer Integer:: sqrt() const
        {
            switch(s)
            {
                case Negative: throw Libc::Exception(EDOM, "sqrt of negative %s",CallSign);
                case __Zero__: return 0;
                case Positive:
                    break;
            }
            return n.sqrt();
        }

        Integer Integer:: abs()  const
        {
            switch(s)
            {
                case Negative: return - *this;
                case Positive: return *this;
                case __Zero__: break;
            }
            return 0;
        }


    }

}
