#include "y/apex/natural.hpp"
#include <iostream>
#include <cfloat>

namespace Yttrium
{
    namespace Apex
    {
        namespace
        {
            template <typename T>
            struct N2R
            {
                static const unsigned CYCLES;

                static inline T Get(const Natural &numer, const Natural &denom)
                {
                    static const T tenth(0.1);
                    const Natural  ten(10);
                    Natural   n=numer,q,r;
                    T         f=0;
                    T         x=1;
                    unsigned  i=0;

                    while(true)
                    {
                        Natural::Div(q,r,n,denom);
                        f += x*q.cast<uint64_t>("real factor");

                        if(++i>CYCLES) break;

                        n  = r * ten;
                        x *= tenth;
                    }


                    return f;
                }
            };

            template <> const unsigned N2R<float>::       CYCLES  = FLT_DIG;
            template <> const unsigned N2R<double>::      CYCLES  = DBL_DIG;
            template <> const unsigned N2R<long double>:: CYCLES  = LDBL_DIG;


        }

        template <>
        float Natural::ToReal<float>(const Natural &numer, const Natural &denom)
        {
            return N2R<float>::Get(numer,denom);
        }

        template <>
        double Natural::ToReal<double>(const Natural &numer, const Natural &denom)
        {
            return N2R<double>::Get(numer,denom);
        }


        template <>
        long double Natural::ToReal<long double>(const Natural &numer, const Natural &denom)
        {
            return N2R<long double>::Get(numer,denom);
        }
    }

}

