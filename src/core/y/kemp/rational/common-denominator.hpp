
//! \file

#ifndef Y_Kemp_Rational_Common_Denominator_Included
#define Y_Kemp_Rational_Common_Denominator_Included 1

#include "y/kemp/rational.hpp"


namespace Yttrium
{
    namespace Kemp
    {

        struct CommonDenominator
        {
            //__________________________________________________________________
            //
            //
            //! find common denominator of a range of rational
            //
            //__________________________________________________________________
            template <typename ITERATOR> static inline
            Natural Of(ITERATOR curr, size_t n)
            {
                if(n<=0) return 1;                             // coherent answer
                Natural den = (*curr).denom;                   // initialize
                while(--n > 0)                                 //
                    den = Natural::LCM(den,(*(++curr)).denom); // update
                return den;
            }


            //__________________________________________________________________
            //
            //
            //! Find common denominator of a sequence of rationals
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            Natural Of(SEQUENCE &seq)
            { return Of(seq.begin(),seq.size()); }

            //__________________________________________________________________
            //
            //
            //! Simplify rationals range to UNIT denominator
            //
            //__________________________________________________________________
            template <typename ITERATOR> static inline
            void Simplify(ITERATOR curr, size_t n)
            {
                const Natural den = Of(curr,n);
                while(n-- > 0) *(curr++) *= den;
            }

            //__________________________________________________________________
            //
            //
            //! Simplify or sequence of rationals to UNIT denominator
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            SEQUENCE & Simplify(SEQUENCE &seq)
            { Simplify(seq.begin(),seq.size()); return seq; }

        };
    }

}

#endif

