
//! \file

#ifndef Y_MKL_SVD_Included
#define Y_MKL_SVD_Included 1

#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! SVD decomposition
        //
        //
        //______________________________________________________________________
        template <typename T>
        class SVD
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________

            //! values comparison prototype
            typedef SignType (*Compare)(const T &, const T &);

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SVD();          //!< setup
            virtual ~SVD() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            //! build SVD decomposition a = u . w . v'
            /**
             \param a [1..m][1..n], replaced by u ou output
             \param w [1..n]        the singular values
             \param v [1..n][1..m]  the svd vectors
             */
            //
            //__________________________________________________________________
            bool build(Matrix<T> &a, Writable<T> &w, Matrix<T> &v);


            //! Sort singular values d[1..n], COLUMN eigenvectors v[1..n][1..n] and matching COLUMN vectors u[1..m][1..n]
            /**
             \param u        left hand side
             \param d        singular values
             \param v        singular vectors
             \param compare  comparison for eigenvalues
             */
            static void Sort(Matrix<T> &u, Writable<T> &d, Matrix<T> &v, Compare compare);

            //! Sort singular values by increasing absolute value: kernel first
            static void Sort(Matrix<T> &u, Writable<T> &d, Matrix<T> &v);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SVD);
            class Code;
            Code *code;
        };

    }

}


#endif
