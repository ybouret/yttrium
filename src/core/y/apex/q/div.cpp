#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Rational Rational:: Div(const Rational &a, const Rational &b)
        {
            SignType s = __Zero__;
            
            switch( Sign::MakePair(a.numer.s,b.numer.s))
            {
                case NZ_Signs:
                case PZ_Signs:
                case ZZ_Signs:
                    throw Specific::Exception(CallSign, "Rational/Rational division by zero");

                case ZN_Signs:
                case ZP_Signs:
                    return 0;

                case NN_Signs:
                case PP_Signs:
                    s = Positive;
                    break;

                case NP_Signs:
                case PN_Signs:
                    s = Negative;
                    break;
            }

            const Integer num = a.numer * b.denom; assert(0!=num);
            const Integer den = a.denom * b.numer; assert(0!=den);
            Rational      res(num.n,den.n);        assert(0!=res.numer);
            Coerce(res.numer.s) = s;
            return res;
        }

    }

}


namespace Yttrium
{
    namespace Apex
    {

        Rational Rational:: Div(const Rational &a, const Integer &b)
        {
            SignType s = __Zero__;

            switch( Sign::MakePair(a.numer.s,b.s))
            {
                case NZ_Signs:
                case PZ_Signs:
                case ZZ_Signs:
                    throw Specific::Exception(CallSign, "Rational/Integer division by zero");

                case ZN_Signs:
                case ZP_Signs:
                    return 0;

                case NN_Signs:
                case PP_Signs:
                    s = Positive;
                    break;

                case NP_Signs:
                case PN_Signs:
                    s = Negative;
                    break;
            }
            /**/                                assert(a.numer!=0);
            const Integer den = a.denom * b;    assert(0!=den);
            Rational      res(a.numer.n,den.n); assert(0!=res.numer);
            Coerce(res.numer.s) = s;
            return res;
        }


        Rational Rational:: Div(const Integer &a, const Rational &b)
        {
            SignType s = __Zero__;

            switch( Sign::MakePair(a.s,b.numer.s))
            {
                case NZ_Signs:
                case PZ_Signs:
                case ZZ_Signs:
                    throw Specific::Exception(CallSign, "Integer/Rational division by zero");

                case ZN_Signs:
                case ZP_Signs:
                    return 0;

                case NN_Signs:
                case PP_Signs:
                    s = Positive;
                    break;

                case NP_Signs:
                case PN_Signs:
                    s = Negative;
                    break;
            }

            const Integer num = a * b.denom;  assert(0!=num);
            Rational      res(num,b.numer.n);
            Coerce(res.numer.s) = s;
            return res;
        }

    }

}


