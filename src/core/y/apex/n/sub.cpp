
#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
namespace Yttrium
{
    namespace Apex
    {
        Natural operator-(const Natural &lhs, const Natural &rhs)
        {
            return Natural(Prototype::Sub( CONST_PROTO(lhs.impl), CONST_PROTO(rhs.impl)),AsImpl);
        }

        Natural operator-(const Natural &lhs, const uint64_t rhs)
        {
            return Natural(Prototype::Add(CONST_PROTO(lhs.impl),rhs),AsImpl);
        }

        Natural operator-(const uint64_t lhs, const Natural &rhs)
        {
            return Natural(Prototype::Sub(lhs,CONST_PROTO(rhs.impl)),AsImpl);
        }

        Natural & Natural:: operator-=(const uint64_t rhs)
        {
            Natural res( Prototype::Sub(CONST_PROTO(impl),rhs),AsImpl);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator-=(const Natural &rhs)
        {
            Natural res( Prototype::Sub(CONST_PROTO(impl),CONST_PROTO(rhs.impl)), AsImpl);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator--()
        {
            Natural res( Prototype::Sub1(CONST_PROTO(impl)), AsImpl);
            xch(res);
            return *this;
        }

        Natural   Natural:: operator--(int)
        {
            const Natural old(*this);
            {
                Natural res( Prototype::Sub1(CONST_PROTO(impl)), AsImpl);
                xch(res);
            }
            return old;
        }

    }
    
}
