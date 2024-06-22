#include "y/apex/rational.hpp"
#include "y/mkl/xreal.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace
        {
            template <typename T>
            struct Q2R
            {
                static inline T Get(const Rational &q)
                {
                    switch(q.numer.s)
                    {
                        case __Zero__: break;
                        case Positive: return  Natural::ToReal<T>(q.numer.n,q.denom);
                        case Negative: return -Natural::ToReal<T>(q.numer.n,q.denom);
                    }
                    return 0;
                }
            };

        }

        template <>
        float Rational:: toReal<float>() const
        {
            return Q2R<float>::Get(*this);
        }


        template <>
        XReal<float> Rational:: toReal< XReal<float> >() const
        {
            return Q2R<float>::Get(*this);

        }


        template <>
        double Rational:: toReal<double>() const
        {
            return Q2R<double>::Get(*this);
        }


        template <>
        XReal<double> Rational:: toReal< XReal<double> >() const
        {
            return Q2R<double>::Get(*this);

        }

        template <>
        long double Rational:: toReal<long double>() const
        {
            return Q2R<long double>::Get(*this);
        }

        template <>
        XReal<long double> Rational:: toReal< XReal<long double> >() const
        {
            return Q2R<long double>::Get(*this);

        }



    }

}
