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

                    case 1: { apq &q = Coerce(*curr); if(__Zero__!=q.numer.s) q = 1;}
                        return;

                    default:
                        break;
                }

                //--------------------------------------------------------------
                //
                // counting signs and computing common denomitor
                //
                //--------------------------------------------------------------
                Natural  g = 0; // final scaling facotr
                {
                    //----------------------------------------------------------
                    //
                    // counting signs and computing common denomitor
                    //
                    //----------------------------------------------------------
                    assert(size>=2);
                    size_t   numPos    = 0;
                    size_t   numNeg    = 0;
                    SignType firstSign = __Zero__;
                    Natural  commonDen = Dispatch(numPos,numNeg,firstSign,*curr);
                    {
                        ITERATOR temp = curr;
                        for(size_t i=size;i>1;--i)
                            commonDen = Natural::LCM(commonDen,Dispatch(numPos,numNeg,firstSign,*(++temp)));
                    }

                    //----------------------------------------------------------
                    //
                    // update
                    //
                    //----------------------------------------------------------
                    switch( Sign::Of(numPos,numNeg) )
                    {
                        case Negative: assert(numPos<numNeg);
                            MulByNeg(commonDen,curr,size,g);
                            break;

                        case Positive: assert(numPos>numNeg);
                            MulByPos(commonDen,curr,size,g);
                            break;

                        case __Zero__: assert(numPos==numNeg);
                            switch(firstSign)
                            {
                                case __Zero__: break; // all zero
                                case Positive: MulByPos(commonDen,curr,size,g); break;
                                case Negative: MulByNeg(commonDen,curr,size,g); break;
                            }
                            break;
                    }

                }

                //--------------------------------------------------------------
                //
                // Normalize
                //
                //--------------------------------------------------------------
                if(g>1)
                {
                    for(size_t i=size;i>0;--i,++curr)
                    {
                        const apq &q = (*curr); assert(1==q.denom);
                        switch(q.numer.s)
                        {
                            case __Zero__: continue;
                            case Negative:
                            case Positive:
                                Coerce(q.numer.n) /= g;
                                continue;
                        }
                    }
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

            template <typename LHS, typename RHS> static inline
            bool AreColinear(LHS &lhs, RHS &rhs)
            {
                const size_t n = lhs.size(); assert( lhs.size() == rhs.size() );

                SignType s = __Zero__;
                Natural  numer, denom;
                for(size_t i=n;i>0;--i)
                {
                    apz l = lhs[i];
                    apz r = rhs[i];
                    switch( Sign::MakePair(l.s,r.s) )
                    {
                            //--------------------------------------------------
                            // zero in front of not zero => false
                            //--------------------------------------------------
                        case ZP_Signs:
                        case ZN_Signs:
                        case PZ_Signs:
                        case NZ_Signs:
                            return false;

                        case ZZ_Signs:
                            continue;

                            //--------------------------------------------------
                            // produce a positive factor
                            //--------------------------------------------------
                        case NN_Signs:
                        case PP_Signs:
                            switch(s)
                            {
                                case Negative: // => false
                                    return false;
                                case __Zero__: // initialize to positive value
                                    s     = Positive;
                                    Natural::Simplify(numer=l.n,denom=r.n);
                                    break;
                                case Positive: // test propto
                                    if(r.n*numer != l.n * denom) return false;
                                    break;

                            }
                            break;

                            //--------------------------------------------------
                            // produce a negative factor
                            //--------------------------------------------------
                        case NP_Signs:
                        case PN_Signs:
                            switch(s)
                            {
                                case Positive: // => false
                                    return false;
                                case __Zero__: // initialize to negative value
                                    s     = Negative;
                                    Natural::Simplify(numer=l.n,denom=r.n);
                                    break;
                                case Negative: // test propto
                                    if(r.n*numer != l.n * denom) return false;
                                    break;
                            }
                            break;

                    }
                }
                
                return true;
            }

        private:
            static const Natural & Dispatch(size_t &numPos, size_t &numNeg, SignType &firstSign, const apq &q) noexcept;

            static inline
            void updateGCD(Natural &g, const apq &q)
            {
                const Natural &rhs = q.numer.n;
                if(rhs>0)
                {
                    if(g<=0)
                        g = rhs;
                    else
                        g = Natural::GCD(g,rhs);
                }
            }


            template <typename ITERATOR> static inline
            void MulByPos(const Natural &common, ITERATOR curr, size_t n, Natural &g)
            {
                assert(0==g);
                while(n-- > 0)
                {
                    apq &q = Coerce(*curr);
                    q *= common;
                    updateGCD(g,q);
                    ++curr;
                    assert(1==q.denom);
                }
            }

            template <typename ITERATOR> static inline
            void MulByNeg(const Natural &common, ITERATOR curr, size_t n, Natural &g)
            {
                assert(0==g);
                while(n-- > 0)
                {
                    apq &q = Coerce(*curr);
                    q *= common;
                    updateGCD(g,q);
                    Sign::ReplaceByOpposite( Coerce(q.numer.s) );
                    ++curr;
                    assert(1==q.denom);
                }
            }
        };

    }

}

#endif
