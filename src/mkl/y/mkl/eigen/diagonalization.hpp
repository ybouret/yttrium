//! \file

#ifndef Y_MKL_Diagonalization_Included
#define Y_MKL_Diagonalization_Included 1


#include "y/container/matrix.hpp"
#include "y/mkl/complex.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Eigen
        {

            template <typename T>
            class Values
            {
            public:
                inline Values(const Readable<T>            &_,
                              const Readable< Complex<T> > &__) noexcept :
                wr(_), wc(__) {}

                inline ~Values() noexcept {}

                const Readable<T>            &wr;
                const Readable< Complex<T> > &wc;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Values);
            };

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
                 \param a  a real matrix reduced to its Hessenberg form: destructed on output
                 \param wr an array that will be filled with the real parts
                 \param wi an array that will be filled with the imagnary parts
                 \param nr the number or real eigenvalues
                 wi[1..nr]=0 and wr[1..nr] are sorted by increasing order.
                 */
                bool QR( Matrix<T> &a, Writable<T> &wr, Writable<T> &wi, size_t &nr);

                //! find the eigen value
                /**
                 \param a a real matrix, that will be balanced and reduced
                 \param wr output sequence of real    eigenvalues
                 \param wc output sequence of complex eigenvalues
                 \return values on success, NULL on error
                 */
                const Values<T> * eig(Matrix<T> &a);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Diagonalization);
                class Code;
                Code *code;
            };

        }

    }

}

#endif

