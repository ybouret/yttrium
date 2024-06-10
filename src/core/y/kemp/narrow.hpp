//! \file

#ifndef Y_Kemp_Sora_Included
#define Y_Kemp_Sora_Included 1

#include "y/kemp/colinearity.hpp"
#include "y/kemp/count-non-zero.hpp"
#include "y/container/matrix.hpp"
#include "y/data/small/heavy/list/bare.hpp"


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
            void Down(Matrix<T>       &target,
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
                    const MatrixRow<U> &src = source[i];  if( CountNonZero::In(src) <= 0 ) continue;
                    bool                bad = false;
                    for(const INode *node=indx.head;node;node=node->next)
                    {
                        const size_t k = **node;
                        if( Colinearity::Of(src,source[k]) )
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
