
#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
namespace Yttrium
{
    namespace Apex
    {

        Natural Natural:: operator-() const
        {
            if( bits() ) throw Specific::Exception(CallSign,"unary minus on positive natural");
            return Natural();
        }

        Natural operator-(const Natural &lhs, const Natural &rhs)
        {
            return Natural(Prototype::Sub( CONST_PROTO(lhs), CONST_PROTO(rhs)),AsImpl);
        }

        Natural operator-(const Natural &lhs, const uint64_t rhs)
        {
            return Natural(Prototype::Add(CONST_PROTO(lhs),rhs),AsImpl);
        }

        Natural operator-(const uint64_t lhs, const Natural &rhs)
        {
            return Natural(Prototype::Sub(lhs,CONST_PROTO(rhs)),AsImpl);
        }

        Natural & Natural:: operator-=(const uint64_t rhs)
        {
            Natural res( Prototype::Sub(CONST_PROTO(*this),rhs),AsImpl);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator-=(const Natural &rhs)
        {
            Natural res( Prototype::Sub(CONST_PROTO(*this),CONST_PROTO(rhs)), AsImpl);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator--()
        {
            Natural res( Prototype::Sub1(CONST_PROTO(*this)), AsImpl);
            xch(res);
            return *this;
        }

        Natural   Natural:: operator--(int)
        {
            const Natural old(*this);
            {
                Natural res( Prototype::Sub1(CONST_PROTO(*this)), AsImpl);
                xch(res);
            }
            return old;
        }

    }
    
}
