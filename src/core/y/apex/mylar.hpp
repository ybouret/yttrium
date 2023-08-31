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


            static const Natural & Dispatch(size_t &numPos, size_t &numNeg, const apq &q) noexcept;

            template <typename ITERATOR> static inline
            void MulByPos(const Natural &common, ITERATOR curr, size_t n)
            {
                while(n-- > 0)
                {
                    apq &q = *curr;
                    q *= common;
                    ++curr;
                    assert(1==q.denom);
                }
            }

            template <typename ITERATOR> static inline
            void MulByNeg(const Natural &common, ITERATOR curr, size_t n)
            {
                while(n-- > 0)
                {
                    apq &q = *curr;
                    q *= common;
                    Sign::ReplaceByOpposite( Coerce(q.numer.s) );
                    ++curr;
                    assert(1==q.denom);
                }
            }



            template <typename ITERATOR> static inline
            void Univocal(ITERATOR curr, const size_t n)
            {
                //--------------------------------------------------------------
                //
                // sorting out cases
                //
                //--------------------------------------------------------------
                switch(n)
                {
                    case 0:
                        return;

                    case 1: { apq &q = *curr; if(__Zero__!=q.numer.s) q = 1;}
                        return;

                    default:
                        break;
                }

                //--------------------------------------------------------------
                //
                // counting signs and computing common denomitar
                //
                //--------------------------------------------------------------
                assert(n>=2);
                size_t   numPos = 0;
                size_t   numNeg = 0;
                Natural  common = Dispatch(numPos,numNeg,*curr);
                {
                    ITERATOR temp = curr;
                    for(size_t i=n;i>1;--i)
                        common = Natural::LCM(common,Dispatch(numPos,numNeg,*(++temp)));
                }
                
                //--------------------------------------------------------------
                //
                // update
                //
                //--------------------------------------------------------------
                if(numNeg>numPos)
                {
                    MulByNeg(common,curr,n);
                }
                else
                {
                    MulByPos(common,curr,n);
                }

            }

            template <typename SEQUENCE> static inline
            void Univocal(SEQUENCE &seq)
            {
                Univocal(seq.begin(),seq.size());
            }
            

        };

    }

}

#endif
