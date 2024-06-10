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
        //! narrowing a matrix to non-colinear, non-zero rows
        //
        //
        //______________________________________________________________________
        struct Narrow
        {

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
                            // zero w.r.t not zero => false
                            //--------------------------------------------------
                        case ZP_Signs:
                        case ZN_Signs:
                        case PZ_Signs:
                        case NZ_Signs:
                            return false;

                            //--------------------------------------------------
                            // zero w.r.t zero => ok
                            //--------------------------------------------------
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
                for(size_t i=source.size();i>0;--i) { if(0!=source[i]) ++count; }
                return count;
            }


            typedef Small::BareHeavyList<size_t> IList; //!< list of indices
            typedef IList::NodeType              INode; //!< nodes for IList


            //__________________________________________________________________
            //
            //
            //! keep only non trivially colinear row vectors
            /**
             \param target compatible with integer type
             \param source matrix of integer rows
             */
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
                    const MatrixRow<U> &src = source[i];  if( CountNonZero(src) <= 0 ) continue;
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


            
        };

    }

}

#endif
