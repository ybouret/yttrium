//! \file

#ifndef Y_MKL_Jacobi_Included
#define Y_MKL_Jacobi_Included 1


#include "y/container/matrix.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Eigen
        {
            template <typename T>
            class Jacobi
            {
            public:
                explicit Jacobi();
                virtual ~Jacobi() noexcept;
                
                //! diagonalize a
                /**
                 \param a squared (digonal) matrux
                 \param d eigenvalues
                 \param v eigenvectors
                 */
                bool build(Matrix<T> &a, Writable<T> &d, Matrix<T> &v);
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Jacobi);
                class Code;
                Code *code;
            };
        }
    }

}

#endif

