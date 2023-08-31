//! \file

#ifndef Y_Apex_Mylar_Included
#define Y_Apex_Mylar_Included 1

#include "y/apex/rational.hpp"
#include <iostream>


namespace Yttrium
{
    namespace Apex
    {
        //______________________________________________________________________
        //
        //
        //
        //! Mylar Yields Linear Algebra for Rationals
        //
        //
        //______________________________________________________________________
        struct Mylar
        {

            //__________________________________________________________________
            //
            //
            //! find common denominator of a range
            //
            //__________________________________________________________________
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


            //__________________________________________________________________
            //
            //
            //! find common denominator of a sequence
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            Natural CommonDenominator(SEQUENCE &seq)
            {
                return CommonDenominator(seq.begin(),seq.size());
            }

            //__________________________________________________________________
            //
            //
            //! Simplify a range
            //
            //__________________________________________________________________
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

            //__________________________________________________________________
            //
            //
            //! Simplify a sequence
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            void Simplify(SEQUENCE &seq)
            {
                Simplify(seq.begin(),seq.size());
            }





            //__________________________________________________________________
            //
            //
            //! Make univocal range
            //
            //__________________________________________________________________
            template <typename ITERATOR> static inline
            void Univocal(ITERATOR curr, const size_t size)
            {
                //--------------------------------------------------------------
                //
                // sorting out cases
                //
                //--------------------------------------------------------------
                switch(size)
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
                assert(size>=2);
                size_t   numPos    = 0;
                size_t   numNeg    = 0;
                SignType firstSign = __Zero__;
                Natural  common = Dispatch(numPos,numNeg,firstSign,*curr);
                {
                    ITERATOR temp = curr;
                    for(size_t i=size;i>1;--i)
                        common = Natural::LCM(common,Dispatch(numPos,numNeg,firstSign,*(++temp)));
                }
                
                //--------------------------------------------------------------
                //
                // update
                //
                //--------------------------------------------------------------
                switch( Sign::Of(numPos,numNeg) )
                {
                    case Negative: assert(numPos<numNeg);
                        MulByNeg(common,curr,size);
                        break;

                    case Positive: assert(numPos>numNeg);
                        MulByPos(common,curr,size);
                        break;

                    case __Zero__: assert(numPos==numNeg);
                        switch(firstSign)
                        {
                            case __Zero__: break; // all zero
                            case Positive: MulByPos(common,curr,size); break;
                            case Negative: MulByNeg(common,curr,size); break;
                        }
                        break;
                }


            }

            //__________________________________________________________________
            //
            //
            //! Make univocal sequence
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            void Univocal(SEQUENCE &seq)
            {
                Univocal(seq.begin(),seq.size());
            }

        private:
            static const Natural & Dispatch(size_t &numPos, size_t &numNeg, SignType &firstSign, const apq &q) noexcept;


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
        };

    }

}

#endif
