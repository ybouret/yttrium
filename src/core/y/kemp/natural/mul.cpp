

#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        bool Natural:: MulByFFT = false;

        static inline
        const Element::BinaryAPI & mapi() noexcept
        {
            if(Natural::MulByFFT) return Element::MulFFT;
            return Element::Mul[ Natural::Strategy ];
        }
        

        Natural & Natural:: operator*=(const Natural &rhs)
        {
            make( mapi() ( **this, *rhs) );
            return *this;
        }

        Natural & Natural:: operator*=(const uint64_t rhs)
        {
            make( mapi() ( **this, rhs) );
            return *this;
        }


        Natural operator*(const Natural &lhs, const Natural &rhs)
        {
            return Natural( mapi() (*lhs,*rhs),AsElement);
        }

        Natural operator*(const uint64_t lhs, const Natural &rhs)
        {
            return Natural(mapi() (lhs,*rhs),AsElement);
        }

        Natural operator*(const Natural &lhs, const uint64_t rhs)
        {
            return Natural(mapi()(*lhs,rhs),AsElement);
        }



        Natural Natural::Sqr(const Natural &n)
        {
            return Natural( Element::Sqr[Strategy](*n), AsElement );
        }

        Natural Natural:: sqr() const
        {
            return Sqr(*this);
        }

    }


}
