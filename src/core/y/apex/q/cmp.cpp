#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        bool operator==(const Rational &a, const Rational &b) noexcept
        {
            return (a.denom == b.denom) && (a.numer == b.numer);
        }

        bool operator==(const Rational &a, const integer_t b) noexcept
        {
            return 1 == a.denom && a.numer == b;
        }

        bool operator==(const integer_t a, const Rational &b) noexcept
        {
            return 1 == b.denom && a == b.numer;
        }

        bool operator==(const Rational &a, const Integer &b) noexcept
        {
            return 1 == a.denom && a.numer == b;
        }

        bool operator==(const Integer &a, const Rational &b) noexcept
        {
            return 1 == b.denom && a == b.numer;
        }

    }

}

namespace Yttrium
{
    namespace Apex
    {
        bool operator!=(const Rational &a, const Rational &b) noexcept
        {
            return (a.denom != b.denom) ||  (a.numer != b.numer) ;
        }

        bool operator!=(const Rational &a, const integer_t b) noexcept
        {
            return 1 != a.denom || a.numer != b;
        }

        bool operator!=(const integer_t a, const Rational &b) noexcept
        {
            return 1 != b.denom || a != b.numer;
        }

        bool operator!=(const Rational &a, const Integer &b) noexcept
        {
            return (1 != a.denom) || (a.numer != b);
        }

        bool operator!=(const Integer &a, const Rational &b) noexcept
        {
            return (1 != b.denom) || (a != b.numer);
        }

    }

}

namespace Yttrium
{
    namespace Apex
    {

        static inline SignType qqCompare(const bool takeOpposite,
                                         const Rational &lhs,
                                         const Rational &rhs)
        {
            assert(lhs.numer.s == rhs.numer.s);
            const Integer L = lhs.numer * rhs.denom;
            const Integer R = rhs.numer * lhs.denom;
            const Integer D = L-R;
            return takeOpposite ? Sign::Opposite(D.s) : D.s;
        }

        SignType Rational:: Compare(const Rational &lhs, const Rational &rhs)
        {
            switch( Sign::MakePair(lhs.numer.s,rhs.numer.s) )
            {

                case NZ_Signs: assert(1==rhs.denom);
                case NP_Signs:
                case ZP_Signs:
                    return Negative;

                case PP_Signs: return qqCompare(false, lhs, rhs);
                case NN_Signs: return qqCompare(true,  lhs, rhs);

                case PZ_Signs:
                case PN_Signs:
                case ZN_Signs:
                    return Positive;

                case ZZ_Signs:
                    assert(1==lhs.denom);
                    assert(1==rhs.denom);
                    break; // => __Zero__
            }
            return __Zero__;
        }
    }

}

namespace Yttrium
{
    namespace Apex
    {
        static inline SignType qzCompare(const bool takeOpposite,
                                         const Rational &q,
                                         const Integer &z)
        {
            const Integer R = q.denom * z;
            const Integer D = q.numer - R;
            return takeOpposite ? Sign::Opposite(D.s) : D.s;
        }

        SignType Rational:: Compare(const Rational &q, const Integer  &z)
        {
            switch( Sign::MakePair(q.numer.s,z.s) )
            {

                case NZ_Signs: 
                case NP_Signs:
                case ZP_Signs:
                    return Negative;

                case PP_Signs: return qzCompare(false, q, z);
                case NN_Signs: return qzCompare(true, q, z);

                case PZ_Signs:
                case PN_Signs:
                case ZN_Signs:
                    return Positive;

                case ZZ_Signs:
                    assert(1==q.denom);
                    break; // => __Zero__
            }
            return __Zero__;
        }

        SignType Rational:: Compare(const Integer  &z, const Rational &q)
        {
            return Sign::Opposite( Compare(q,z) );
        }

    }
}

