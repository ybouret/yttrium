
//! \file

#ifndef Y_MKL_Algebra_Cyclic_Included
#define Y_MKL_Algebra_Cyclic_Included 1

#include "y/mkl/algebra/tridiag.hpp"

namespace Yttrium
{
    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //
        //! Cyclic (a,b,c)
        //
        //
        //__________________________________________________________________
        template <typename T>
        class Cyclic : public TriDiag<T>
        {
        public:
            explicit Cyclic(const size_t n); //!< n >= 2
            virtual ~Cyclic() noexcept;




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cyclic);
            class  Code;
            Code  *code;
        public:
            T &    alpha;
            T &    beta;
        };

    }

}

#endif
