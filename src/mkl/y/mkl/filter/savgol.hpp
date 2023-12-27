
//! \file

#ifndef Y_MKL_Filter_SavGol_Included
#define Y_MKL_Filter_SavGol_Included 1


#include "y/mkl/filter/resago.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace MKL
    {
        typedef ArcPtr<ReSaGo::Factory> SavGolFactory;

        template <typename T>
        class SavGol
        {
        public:
            explicit SavGol(const SavGolFactory &factory);
            virtual ~SavGol() noexcept;

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

