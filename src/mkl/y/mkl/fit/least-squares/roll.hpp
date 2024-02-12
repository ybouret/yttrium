
//! \file

#ifndef Y_Fit_Compute_Least_Squares_Roll_Included
#define Y_Fit_Compute_Least_Squares_Roll_Included 1

#include "y/mkl/fit/least-squares.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {
            //__________________________________________________________________
            //
            //
            //! List of LeastSquares to fit multiple samples
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class LeastSquaresRoll : public Object, public LeastSquares<ABSCISSA,ORDINATE>::List
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef LeastSquares<ABSCISSA,ORDINATE> LeastSquaresType; //!< alias
                typedef typename LeastSquaresType::List ListType;         //!< alias
                typedef CxxPoolOf<LeastSquaresType>     PoolType;         //!< alias

                using ListType::size;
                using ListType::popTail;
                using ListType::pushTail;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup empty
                inline explicit LeastSquaresRoll() noexcept : ListType(), pool() {}

                //! cleanup
                inline virtual ~LeastSquaresRoll() noexcept {}

                //! setup for multiple samples
                inline LeastSquaresRoll &setup(const size_t numSamples) {
                    while(size>numSamples) pool.store( popTail() );
                    while(size<numSamples) pushTail( (pool.size>0) ? pool.query() : new LeastSquaresType() );
                    assert(numSamples==size);
                    return *this;
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                PoolType pool; //!< pool of available LeastSquares

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquaresRoll);
            };

        }

    }

}

#endif
