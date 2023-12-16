
//! \file

#ifndef Y_MKL_SVD_Included
#define Y_MKL_SVD_Included 1

#include "y/container/matrix.hpp"
#include "y/comparison.hpp"

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
            // C++
            //
            //__________________________________________________________________
            explicit SVD();          //!< setup
            virtual ~SVD() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            //! build SVD decomposition
            //
            //__________________________________________________________________
            bool build(Matrix<T> &a, Writable<T> &w, Matrix<T> &v);


            //! Sort singular values d[1..n], COLUMN eigenvectors v[1..n][1..n] and matching COLUMN vectors u[1..m][1..n]
            /**
             \param u        left hand sive
             \param d        eigenvalues
             \param v        eigenvectors, colums
             \param compare comparison for eigenvalues
             */
            template <typename COMPARE> static inline
            void Sort(Matrix<T> &u, Writable<T> &d, Matrix<T> &v, COMPARE &compare)
            {
                assert(v.rows==v.cols);
                assert(v.rows==d.size());
                assert(u.cols==d.size());

                const size_t n = d.size();
                for(size_t i=1;i<n;i++)
                {
                    size_t k=i;
                    T p=d[i];
                    for(size_t j=i+1;j<=n;++j)
                    {
                        if( Negative == compare(d[j],p) )
                            p = d[k=j];
                    }

                    if(k != i)
                    {
                        d[k]=d[i];
                        d[i]=p;
                        v.swapCols(k,i);
                        u.swapCols(k,i);
                    }
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SVD);
            class Code;
            Code *code;
        };

    }

}


#endif
