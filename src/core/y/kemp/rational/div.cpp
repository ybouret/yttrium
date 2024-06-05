
#include "y/kemp/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        void Rational:: DivisionByZero()
        {
            throw Specific::Exception(CallSign, "Division By Zero");
        }

        static inline
        Rational DivAsV1(const SignType s, const Rational &lhs, const Rational &rhs)
        {
            const Natural num = lhs.numer.n * rhs.denom;
            const Natural den = lhs.denom   * rhs.numer.n;
            return Rational(s,num,den);
        }

        Rational Rational:: Div(const Rational &lhs, const Rational &rhs)
        {
            switch( Sign::MakePair(lhs.numer.s, rhs.numer.s) )
            {
                    // division by zero
                case ZZ_Signs:
                case NZ_Signs:
                case PZ_Signs:
                    break;

                    // trivial cases
                case ZP_Signs:
                case ZN_Signs:
                    return Rational();

                    // positive
                case PP_Signs:
                case NN_Signs:
                    return DivAsV1(Positive,lhs,rhs);

                    // negative
                case PN_Signs:
                case NP_Signs:
                    return DivAsV1(Negative,lhs,rhs);
            }
            DivisionByZero();
            return Rational();
        }

    }
}

namespace Yttrium
{
    namespace Kemp
    {

        static inline
        Rational DivAsV2(const SignType s, const Rational &lhs, const Integer &rhs)
        {
            const Natural den = lhs.denom   * rhs.n;
            return Rational(s,lhs.numer.n,den);
        }

        Rational Rational:: Div(const Rational &lhs, const Integer &rhs)
        {
            switch( Sign::MakePair(lhs.numer.s, rhs.s) )
            {
                    // division by zero
                case ZZ_Signs:
                case NZ_Signs:
                case PZ_Signs:
                    break;

                    // trivial cases
                case ZP_Signs:
                case ZN_Signs:
                    return Rational();

                    // positive
                case PP_Signs:
                case NN_Signs:
                    return DivAsV2(Positive,lhs,rhs);

                    // negative
                case PN_Signs:
                case NP_Signs:
                    return DivAsV2(Negative,lhs,rhs);
            }
            DivisionByZero();
            return Rational();
        }


    }

}


namespace Yttrium
{
    namespace Kemp
    {
        Rational Rational:: Div(const Rational &lhs, const Natural &rhs)
        {
            if(rhs.sign() == __Zero__) DivisionByZero();

            const Natural den = lhs.denom * rhs;
            return Rational(lhs.numer,den);
        }
    }

}



namespace Yttrium
{
    namespace Kemp
    {

        static inline
        Rational DivAsV3(const SignType s, const Rational &lhs, const uint64_t &rhs)
        {
            const Natural den = lhs.denom * rhs;
            return Rational(s,lhs.numer.n,den);
        }

        Rational Rational:: Div(const Rational &lhs, const int64_t rhs)
        {
            uint64_t       u = 0;
            const SignType s = Sign::Of(rhs);
            switch(s)
            {
                case __Zero__: break;
                case Negative: u = static_cast<uint64_t>(-rhs); goto DIV;
                case Positive: u = static_cast<uint64_t>( rhs); goto DIV;
            }
            DivisionByZero();
            return Rational();

        DIV:
            switch( Sign::MakePair(lhs.numer.s,s) )
            {
                    // division by zero
                case ZZ_Signs:
                case NZ_Signs:
                case PZ_Signs:
                    break;

                    // trivial cases
                case ZP_Signs:
                case ZN_Signs:
                    return Rational();

                    // positive
                case PP_Signs:
                case NN_Signs:
                    return DivAsV3(Positive,lhs,u);

                    // negative
                case PN_Signs:
                case NP_Signs:
                    return DivAsV3(Negative,lhs,u);
            }
            DivisionByZero();
            return Rational();
        }


    }

}
