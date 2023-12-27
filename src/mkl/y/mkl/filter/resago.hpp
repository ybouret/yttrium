//! \file

#ifndef Y_MKL_Filter_ReSaGo_Included
#define Y_MKL_Filter_ReSaGo_Included 1


#include "y/mkl/api.hpp"
#include "y/container/matrix.hpp"
#include "y/apex/rational.hpp"
#include "y/counted.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! Regular Savitzky-Golay Filters
        //
        //
        //______________________________________________________________________
        struct ReSaGo
        {

            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t MaxKeptDegree = 2; //!< keep degree 0...MaxKeptDegree in matrix

            //__________________________________________________________________
            //
            //
            //! Factory of unique filters computed as rational
            //
            //__________________________________________________________________
            class Factory : public Object, public Counted
            {
            public:
                class Code;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Factory();          //!< setup empty
                virtual ~Factory() noexcept; //!< cleanup

                //! compute rational filter weights
                /**
                 \param nLeft  number of points at the left  of the point of interest
                 \param nRight number of points at the right of the point of interest
                 \return matrix[1..maxDegree+1][1..nLeft+1+nRight]
                 */
                const Matrix<apq> & operator()(const uint32_t nLeft,
                                               const uint32_t nRight,
                                               const uint32_t degree);

                Code * const code;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Factory);

            };


        };
    }

}

#endif
