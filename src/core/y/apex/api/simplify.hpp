
//! \file

#ifndef Y_Apex_Simplify_Included
#define Y_Apex_Simplify_Included 1

#include "y/apex/api/natural-part.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! Simplification of Array/denom
        //
        //
        //______________________________________________________________________
        struct Simplify
        {

            //! forward to overloaded function
            template <typename  T> static inline
            const Natural & Get(const T &_) noexcept {
                return NaturalPart::Of(_);
            }

            //! simplify array
            template <
            typename ARRAY,
            typename DENOM> static inline
            void Array(ARRAY &numer, DENOM &denom)
            {
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                const size_t   len = numer.size(); if(len<=0) return;
                const Natural &den = Get(denom);
                Natural        gcd = Natural::GCD(Get(numer[1]),den);

                //--------------------------------------------------------------
                // find smallest of GCD
                //--------------------------------------------------------------
                for(size_t i=len;i>1;--i) {
                    const Natural tmp = Natural::GCD(Get(numer[i]),den);
                    if(tmp<gcd) gcd=tmp;
                }

                //--------------------------------------------------------------
                // update
                //--------------------------------------------------------------
                Coerce(den) /= gcd;
                for(size_t i=len;i>0;--i)
                {
                    Coerce(Get(numer[i])) /= gcd;
                }

            }

            //! set to unit denominator
            static Natural Array(Writable<Rational> &arr);

            //! set to irreductible
            static Natural Array(Writable<Integer> &arr);

        };

    }

}

#endif
