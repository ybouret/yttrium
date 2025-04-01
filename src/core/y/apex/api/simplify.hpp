
//! \file

#ifndef Y_Apex_Simplify_Included
#define Y_Apex_Simplify_Included 1

#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {

        struct Simplify
        {
            static const Natural & Get(const Natural &);
            static const Natural & Get(const Integer &);

            template <
            typename ARRAY,
            typename DENOM> static inline
            void Array(ARRAY &numer, DENOM &denom)
            {
                // initialize
                const size_t   len = numer.size(); if(len<=0) return;
                const Natural &den = Get(denom);
                Natural        gcd = Natural::GCD(Get(numer[1]),den);


                // find smallest of GCD
                for(size_t i=len;i>1;--i) {
                    const Natural tmp = Natural::GCD( Get(numer[i]),den);
                    if(tmp<gcd) gcd=tmp;
                }

                // update
                Coerce(den) /= gcd;
                for(size_t i=len;i>0;--i)
                {
                    Coerce(Get(numer[i])) /= gcd;
                }

            }




        };

    }

}

#endif
