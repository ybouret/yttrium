
//! \file

#ifndef Y_MKL_Filter_SavGol_Included
#define Y_MKL_Filter_SavGol_Included 1


#include "y/mkl/filter/resago.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //! Shared Factory of rational matrices
        //
        //______________________________________________________________________
        typedef ArcPtr<ReSaGo::Factory> SavGolFactory;

        //______________________________________________________________________
        //
        //
        //
        //! Factory of (extended) floating point matrices
        //
        //
        //______________________________________________________________________
        template <typename T>
        class SavGol
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            //! setup with shared factory of rational matrices
            explicit SavGol(const SavGolFactory &factory);
            virtual ~SavGol() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! get/create matrix from shared factory and convert to T
            const Matrix<T> & operator()(const uint32_t nLeft,
                                         const uint32_t nRight,
                                         const uint32_t degree);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SavGol);
            class Code;
            Code *code;
        };
    }

}

#endif

