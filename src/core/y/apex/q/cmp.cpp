
#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {

        bool operator==(const Rational &lhs, const Rational &rhs) noexcept
        {
            return lhs.numer == rhs.numer && lhs.denom == rhs.denom;
        }

        bool operator==(const Rational &lhs, const int64_t rhs) noexcept
        {
            return lhs.denom == 1 && lhs.numer == rhs;
        }

        bool operator==(const int64_t lhs, const Rational &rhs) noexcept
        {
            return rhs.denom == 1 && lhs==rhs.numer;
        }

        bool operator==(const Rational &lhs, const Integer &rhs) noexcept
        {
            return lhs.denom == 1 && lhs.numer == rhs;
        }

        bool operator==(const Integer &lhs, const Rational &rhs) noexcept
        {
            return rhs.denom == 1 && lhs==rhs.numer;
        }

        bool operator==(const Rational &lhs, const Natural &rhs) noexcept
        {
            return lhs.denom == 1 && lhs.numer == rhs;
        }

        bool operator==(const Natural &lhs, const Rational &rhs) noexcept
        {
            return rhs.denom == 1 && lhs==rhs.numer;
        }
        
    }

}

namespace Yttrium
{
    namespace Apex
    {

        bool operator!=(const Rational &lhs, const Rational &rhs) noexcept
        {
            return lhs.numer != rhs.numer || lhs.denom != rhs.denom;
        }

        bool operator!=(const Rational &lhs, const int64_t rhs) noexcept
        {
            return lhs.denom != 1 || lhs.numer != rhs;
        }

        bool operator!=(const int64_t lhs, const Rational &rhs) noexcept
        {
            return rhs.denom != 1 || lhs!=rhs.numer;
        }

        bool operator!=(const Rational &lhs, const Integer &rhs) noexcept
        {
            return lhs.denom != 1 || lhs.numer != rhs;
        }

        bool operator!=(const Integer &lhs, const Rational &rhs) noexcept
        {
            return rhs.denom != 1 || lhs!=rhs.numer;
        }

        bool operator!=(const Rational &lhs, const Natural &rhs) noexcept
        {
            return lhs.denom != 1 || lhs.numer != rhs;
        }

        bool operator!=(const Natural &lhs, const Rational &rhs) noexcept
        {
            return rhs.denom != 1 || lhs!=rhs.numer;
        }


    }

}

namespace Yttrium
{
    namespace Apex
    {
        SignType Rational:: Cmp(const Rational &lhs, const Rational &rhs)
        {
            const Integer L = lhs.numer * rhs.denom;
            const Integer R = rhs.numer * lhs.denom;
            return Integer::Cmp(L,R);
        }

        SignType Rational::Cmp(const Rational &lhs, const int64_t rhs)
        {
            Integer R = lhs.denom; R *= rhs;
            return Integer::Cmp(lhs.numer,R);
        }

        SignType Rational:: Cmp(const int64_t lhs, const Rational &rhs)
        {
            Integer L = lhs; L *= rhs.denom;
            return Integer::Cmp(L,rhs.numer);
        }

        SignType Rational::Cmp(const Rational &lhs, const Integer &rhs)
        {
            const Integer R = lhs.denom * rhs;
            return Integer::Cmp(lhs.numer,R);
        }

        SignType Rational:: Cmp(const Integer &lhs, const Rational &rhs)
        {
            const Integer L = lhs * rhs.denom;
            return Integer::Cmp(L,rhs.numer);
        }

        SignType Rational::Cmp(const Rational &lhs, const Natural &rhs)
        {
            Integer R = lhs.denom; R *= rhs;
            return Integer::Cmp(lhs.numer,R);
        }

        SignType Rational:: Cmp(const Natural &lhs, const Rational &rhs)
        {
            Integer L = lhs; L *= rhs.denom;
            return Integer::Cmp(L,rhs.numer);
        }

    }

}

