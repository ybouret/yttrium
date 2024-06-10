//! \file

#ifndef Y_Kemp_Sora_Included
#define Y_Kemp_Sora_Included 1

#include "y/kemp/colinearity.hpp"
#include "y/kemp/count-non-zero.hpp"
#include "y/kemp/univocal.hpp"
#include "y/container/matrix.hpp"
#include "y/container/cxx/series.hpp"


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
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxSeries<size_t,Memory::Dyadic> Good;  //!< alias

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
            template <
            typename T,
            typename T_ALLOC,
            typename U,
            typename U_ALLOC> static inline
            void Down(Matrix<T,T_ALLOC>       &target,
                      const Matrix<U,U_ALLOC> &source)
            {
                //--------------------------------------------------------------
                // preparing indices
                //--------------------------------------------------------------
                target.release();
                const size_t cols = source.cols;
                const size_t rows = source.rows;
                Good         good(rows);
                for(size_t i=1;i<=rows;++i)
                {
                    // get current row
                    const MatrixRow<U> &src = source[i];

                    // check not zero
                    if( CountNonZero::In(src) <= 0 ) continue;

                    // check not proportional to already good rows
                    for(size_t j=good.size();j>0;--j)
                        if( Colinearity::Of(src,source[ good[j] ])) goto NEXT;

                    // register a new good row index
                    good << i;
                NEXT:;
                }

                //--------------------------------------------------------------
                // building target
                //--------------------------------------------------------------
                const size_t ng = good.size();
                if(ng>0)
                {
                    assert(cols>0);
                    target.make(ng,cols);
                    for(size_t i=1;i<=ng;++i)
                    {
                        const size_t       k   = good[i];
                        const Readable<U> &src = source[k];
                        MatrixRow<T>      &tgt = target[i];
                        for(size_t j=cols;j>0;--j) tgt[j] = src[j];
                    }
                }
            }


            
        };

    }

}

#endif
