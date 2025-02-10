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

        SignType Rational:: Compare(const Rational &lhs, const Rational &rhs)
        {
            switch( Sign::MakePair(lhs.numer.s,rhs.numer.s) )
            {

                case NZ_Signs: assert(1==rhs.denom);
                case NP_Signs:
                case ZP_Signs:
                    return Negative;

                case PP_Signs:
                case NN_Signs: {
                    const Integer L = lhs.numer * rhs.denom;
                    const Integer R = rhs.numer * lhs.denom;
                    const Integer D = L-R;
                    return D.s;
                }

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


        SignType Rational:: Compare(const Rational &q, const Integer  &z)
        {
            switch( Sign::MakePair(q.numer.s,z.s) )
            {

                case NZ_Signs:
                case NP_Signs:
                case ZP_Signs:
                    return Negative;

                case PP_Signs:
                case NN_Signs:  {
                    const Integer R = q.denom * z;
                    const Integer D = q.numer - R;
                    return D.s;
                }

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

namespace Yttrium
{
    namespace Apex
    {

        SignType Rational:: Compare(const Rational &q, const integer_t z)
        {
            switch( Sign::MakePair(q.numer.s, Sign::Of(z)) )
            {

                case NZ_Signs:
                case NP_Signs:
                case ZP_Signs:
                    return Negative;

                case PP_Signs:
                {
                    assert(z>0);
                    const Integer R = q.denom * static_cast<natural_t>(z);
                    const Integer D = q.numer - R;
                    return D.s;
                }

                case NN_Signs: {
                    assert(z<0);
                    const Integer Z = z;
                    const Integer R = q.denom * static_cast<natural_t>(-z);
                    const Integer D = q.numer+R;
                    return D.s;
                }

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

        SignType Rational:: Compare(const integer_t z,  const Rational &q)
        {
            return Sign::Opposite( Compare(q,z) );
        }
    }
}

namespace Yttrium
{
    namespace Apex
    {
        SignType Rational:: Compare(const Rational &q, const Natural  &n)
        {
            switch(q.numer.s)
            {
                case Negative: return Negative;
                case __Zero__: return n->bits<=0 ? __Zero__ : Negative;
                case Positive:
                    break;
            }
            const Natural R = q.denom * n;
            const Integer D = q.numer - R;
            return D.s;
        }

        SignType Rational:: Compare(const Natural  &n, const Rational &q)
        {
            return Sign::Opposite( Compare(q,n) );
        }
    }
}
