//! \file

#ifndef Y_Apex_Mylar_Included
#define Y_Apex_Mylar_Included 1

#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //! Mylar Yields Linear Algebra for Rationals
        struct Mylar
        {

            template <typename ITERATOR> static inline
            Natural CommonDenominator(ITERATOR curr, size_t n)
            {
                if(n<=0) return 1;
                Natural den = (*curr).denom;
                while(--n > 0)
                {
                    ++curr;
                    den = Natural::LCM(den,(*curr).denom);
                }
                return den;
            }

            template <typename SEQUENCE> static inline
            Natural CommonDenominator(SEQUENCE &seq)
            {
                return CommonDenominator(seq.begin(),seq.size());
            }

            template <typename ITERATOR> static inline
            void Simplify(ITERATOR curr, size_t n)
            {
                const Natural den = CommonDenominator(curr,n);
                while(n-- > 0)
                {
                    (*curr) *= den;
                    ++curr;
                }
            }

            template <typename SEQUENCE> static inline
            void Simplify(SEQUENCE &seq)
            {
                Simplify(seq.begin(),seq.size());
            }


        };

    }

}

#endif
