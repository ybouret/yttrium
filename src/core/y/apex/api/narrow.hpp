
//! \file

#ifndef Y_Apex_Narrow_Included
#define Y_Apex_Narrow_Included 1

#include "y/apex/api/colinearity.hpp"
#include "y/apex/api/univocal.hpp"
#include "y/apex/api/count-non-zero.hpp"


#include "y/sequence/vector.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! Narrow Down a matrix to univocal sub-matrix
        //
        //
        //______________________________________________________________________
        class Narrow
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Narrow() noexcept; //!< setup
            virtual ~Narrow() noexcept; //!< cleanup

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
            typename U_ALLOC>   inline
            void down(Matrix<T,T_ALLOC>       &target,
                      const Matrix<U,U_ALLOC> &source)
            {
                //--------------------------------------------------------------
                // preparing indices
                //--------------------------------------------------------------
                good.free();
                nrm2.free();
                target.release();
                const size_t cols = source.cols;
                const size_t rows = source.rows;

                for(size_t i=1;i<=rows;++i)
                {
                    // get current row
                    const MatrixRow<U> &src = source[i];

                    // check not zero
                    if( CountNonZero(src) <= 0 ) continue;

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
                        nrm2 << Univocal::Make( target[i].ld(source[good[i]]) );
                }
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Vector<size_t,Memory::Dyadic>  good; //!< indices of each row
            Vector<Natural,Memory::Dyadic> nrm2; //!< |target|^2 for each row

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Narrow);
        };

    }

}

#endif

