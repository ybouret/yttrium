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

            //__________________________________________________________________
            //
            //
            //
            //! light weight place holder for internal eigenvalues
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Values
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup from internal readable
                inline Values(const Readable<T>            &_,
                              const Readable< Complex<T> > &__) noexcept :
                wr(_), wc(__) {}

                //! cleanup
                inline ~Values() noexcept {}

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Readable<T>            &wr; //!< internal real values
                const Readable< Complex<T> > &wc; //!< internal complex values

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Values);
            };

            //__________________________________________________________________
            //
            //
            //
            //! Diagonalization algorithms
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Diagonalization
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Diagonalization();           //!< setup internal code
                virtual ~Diagonalization() noexcept;  //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

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
                 \param a a real matrix, preserved
                 \return values on success, NULL on error
                 */
                const Values<T> * eig(const Matrix<T> &a);

                //!
                void guess(Writable<T>     &eVal,
                           Matrix<T>        eVec,
                           const Matrix<T> &a);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Diagonalization);
                class Code;
                Code *code;
            };

        }

    }

}

#endif

