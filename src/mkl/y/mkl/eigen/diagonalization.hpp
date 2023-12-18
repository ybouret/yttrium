
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

                //! find the eigen values
                /**
                 \param a  a real matrix reduced to its Hessenberg form: destructed !
                 \param wr an array that will be filled with the real parts
                 \param wi an array that will be filled with the imagnary parts
                 \param nr the number or real eigenvalues
                 wi[1..nr]=0 and wr[1..nr] are sorted by increasing order.
                 */
                bool QR( Matrix<T> &a, Writable<T> &wr, Writable<T> &wi, size_t &nr);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Diagonalization);
                class Code;
                Code *code;
            };

        }

    }

}

#endif
