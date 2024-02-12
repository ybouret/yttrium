
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

            template <typename ABSCISSA, typename ORDINATE>
            class LeastSquaresRoll : public LeastSquares<ABSCISSA,ORDINATE>::List
            {
            public:
                typedef LeastSquares<ABSCISSA,ORDINATE> LeastSquaresType;
                typedef typename LeastSquaresType::List ListType;
                typedef CxxPoolOf<LeastSquaresType>     PoolType;

                using ListType::size;
                using ListType::popTail;
                using ListType::pushTail;

                inline explicit LeastSquaresRoll() noexcept : ListType(), pool() {}
                inline virtual ~LeastSquaresRoll() noexcept {}

                inline LeastSquaresRoll &setup(const size_t numSamples) {
                    while(size>numSamples) pool.store( popTail() );
                    while(size<numSamples) pushTail( (pool.size>0) ? pool.query() : new LeastSquaresType() );
                    assert(numSamples==size);
                    return *this;
                }

                PoolType pool;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquaresRoll);
            };

        }

    }

}

#endif
