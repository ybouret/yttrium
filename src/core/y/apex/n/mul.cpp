#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
namespace Yttrium
{
    namespace Apex
    {
        Natural operator*(const Natural &lhs, const Natural &rhs)
        {
            Prototype::MulProc mul = Prototype::LongMul;
            return Natural( Prototype::Mul(CONST_PROTO(lhs.impl),CONST_PROTO(rhs.impl),mul,0), AsImpl);
        }

        Natural operator*(const Natural &lhs, const uint64_t rhs)
        {
            return Natural( Prototype::Mul(CONST_PROTO(lhs.impl),rhs), AsImpl);
        }

        Natural operator*(const uint64_t lhs, const Natural &rhs)
        {
            return Natural( Prototype::Mul(CONST_PROTO(rhs.impl),lhs), AsImpl);
        }

        Natural & Natural:: operator*=(const Natural &rhs)
        {
            Natural temp = (*this) * rhs;
            xch(temp);
            return *this;
        }

        Natural & Natural:: operator*=(const uint64_t rhs)
        {
            Natural temp(Prototype::Mul(CONST_PROTO(impl),rhs), AsImpl);
            xch(temp);
            return *this;
        }

    }

}

