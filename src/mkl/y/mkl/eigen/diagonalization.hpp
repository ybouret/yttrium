
//! \file

#ifndef Y_MKL_Diagonalization_Included
#define Y_MKL_Diagonalization_Included 1


#include "y/container/matrix.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Eigen
        {

            template <typename T>
            void Balance(Matrix<T> &a);

            template <typename T>
            class Diagonalization
            {
            public:
                explicit Diagonalization();
                virtual ~Diagonalization() noexcept;

                //! balance a real matrix
                /**
                 Given a matrix a[1..n][1..n], this routine replaces it by a balanced matrix with identical eigenvalues.
                 A symmetric matrix is already balanced and is unaffected by this procedure.
                 */
                void balance(Matrix<T> &a);

                //! reduction to a real Hessenberg form
                /**
                 \param a matrix a[1..n][1..n] should be balanced.
                 */
                void reduce(Matrix<T> &a);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Diagonalization);
                class Code;
                Code *code;
            };

        }

    }

}

#endif
