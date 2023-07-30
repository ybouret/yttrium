//! \file

#ifndef Y_Matrix_Metrics_Included
#define Y_Matrix_Metrics_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Common metrics for Matrices
    //
    //
    //__________________________________________________________________________
    class MatrixMetrics
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const CallSign;       //!< "Matrix"
        static const unsigned     DATA_INDEX = 0; //!< in embedded
        static const unsigned     ROWS_INDEX = 1; //!< in embedded
        static const unsigned     NUM_FIELDS = 2; //!< in embedded

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit MatrixMetrics(const size_t nr, const size_t nc); //!< setup
        virtual ~MatrixMetrics() noexcept;                        //!< setup empty
        explicit MatrixMetrics(const MatrixMetrics &) noexcept;   //!< copy parameters

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        bool isSquare() const noexcept; //!< rows == cols
        bool isValid()  const noexcept; //!< rows>0  ( => cols>0 )
        bool isEmpty()  const noexcept; //!< rows<=0 ( => cols<=0 )

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        const size_t rows; //!< rows
        const size_t cols; //!< columns
        const size_t items; //!< rows*cols

    private:
        Y_DISABLE_ASSIGN(MatrixMetrics);
    };
}

#endif
