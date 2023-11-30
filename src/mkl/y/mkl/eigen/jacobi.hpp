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

            //__________________________________________________________________
            //
            //
            //
            //! Jacobi method for [float|double|long double] and XReal
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Jacobi
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Jacobi();           //!< setup
                virtual ~Jacobi() noexcept;  //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

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

