//! \file

#ifndef Y_Kemp_Sora_Included
#define Y_Kemp_Sora_Included 1

#include "y/kemp/rational.hpp"
#include "y/container/matrix.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include <iostream>


namespace Yttrium
{
    namespace Kemp
    {
        //______________________________________________________________________
        //
        //
        //
        //! Set Of Rationals Arithmetic
        //
        //
        //______________________________________________________________________
        struct Sora
        {

            //__________________________________________________________________
            //
            //
            //! find common denominator of a range of rational
            //
            //__________________________________________________________________
            template <typename ITERATOR> static inline
            Natural CommonDenominator(ITERATOR curr, size_t n)
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
            Natural CommonDenominator(SEQUENCE &seq)
            { return CommonDenominator(seq.begin(),seq.size()); }

            //__________________________________________________________________
            //
            //
            //! Simplify rationals range to UNIT denominator
            //
            //__________________________________________________________________
            template <typename ITERATOR> static inline
            void Simplify(ITERATOR curr, size_t n)
            {
                const Natural den = CommonDenominator(curr,n);
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


            //__________________________________________________________________
            //
            //
            //! Make univocal range of rationals
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
                        // do nothing
                    case 0: return;

                        // left zero untouched or make positive 1
                    case 1: { apq &q = Coerce(*curr); if(__Zero__!=q.numer.s) q = 1;} return;

                        // generic case
                    default: break;
                }

                //--------------------------------------------------------------
                //
                // counting signs and computing common denomitor
                //
                //--------------------------------------------------------------
                Natural  g = 0; // final scaling factor
                {
                    //----------------------------------------------------------
                    //
                    // counting signs and computing common denominator
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
                    // update according to signs majority and and gather GCD
                    //
                    //----------------------------------------------------------
                    switch( Sign::Of(numPos,numNeg) )
                    {
                        case Negative: assert(numPos<numNeg); MulByNeg(commonDen,curr,size,g); break;
                        case Positive: assert(numPos>numNeg); MulByPos(commonDen,curr,size,g); break;
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
                // Final reduction
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
            //! Make univocal sequence of rationals
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            void Univocal(SEQUENCE &seq) { Univocal(seq.begin(),seq.size()); }

            //__________________________________________________________________
            //
            //
            //! Make univocal rows of Integers
            //
            //__________________________________________________________________
            static void MakeUnivocal(Matrix<apz> &a);

            //__________________________________________________________________
            //
            //
            //! Check colinearity of int/unsigned/apz arrays
            //
            //__________________________________________________________________
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
                                    if(r.n*numer != l.n*denom) return false;
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


            //__________________________________________________________________
            //
            //
            //! count number of non-zero elements
            //
            //__________________________________________________________________
            template <typename SOURCE>
            static inline size_t CountNonZero(SOURCE &source)
            {
                size_t count = 0;
                for(size_t i=source.size();i>0;--i)
                { if(0!=source[i]) ++count; }
                return count;
            }


            typedef Small::BareHeavyList<size_t> IList; //!< list of indices
            typedef IList::NodeType              INode; //!< nodes for IList


            //__________________________________________________________________
            //
            //
            //! keep only non trivially colinear row vectors
            //
            //__________________________________________________________________
            template <typename T, typename U> static inline
            void Compress(Matrix<T>       &target,
                          const Matrix<U> &source)
            {
                //--------------------------------------------------------------
                // preparing indices
                //--------------------------------------------------------------
                target.release();
                IList        indx;
                const size_t cols = source.cols;
                const size_t rows = source.rows;
                for(size_t i=1;i<=rows;++i)
                {
                    const MatrixRow<U> &src = source[i];  if( CountNonZero(src) <=0 ) continue;
                    bool                bad = false;

                    for(const INode *node=indx.head;node;node=node->next)
                    {
                        const size_t k = **node;
                        if( AreColinear(src,source[k]) )
                        {
                            bad = true;
                            break;
                        }
                    }
                    
                    if(bad) continue;
                    indx << i;
                }
                if(indx.size>0)
                {
                    assert(cols>0);
                    target.make(indx.size,cols);
                    size_t i=1;
                    for(const INode *node=indx.head;node;node=node->next,++i)
                    {
                        const size_t        k   = **node;
                        MatrixRow<T>       &tgt = target[i];
                        const MatrixRow<U> &src = source[k];
                        for(size_t j=cols;j>0;--j) tgt[j] = src[j];
                    }
                }
            }


        private:
            static const Natural & Dispatch(size_t &numPos, size_t &numNeg, SignType &firstSign, const apq &q) noexcept;
            static inline void     UpdateGCD(Natural &g, const apq &q);

            template <typename ITERATOR> static inline
            void MulByPos(const Natural &common, ITERATOR curr, size_t n, Natural &g)
            {
                assert(0==g);
                while(n-- > 0)
                {
                    apq &q = Coerce(*curr);
                    q *= common;
                    UpdateGCD(g,q);
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
                    UpdateGCD(g,q);
                    Sign::ReplaceByOpposite( Coerce(q.numer.s) );
                    ++curr;
                    assert(1==q.denom);
                }
            }
        };

    }

}

#endif
