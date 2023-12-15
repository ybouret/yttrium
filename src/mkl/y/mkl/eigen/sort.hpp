
//! \file

#ifndef Y_MKL_Eigen_Sort_Included
#define Y_MKL_Eigen_Sort_Included 1


#include "y/container/matrix.hpp"
#include "y/comparison.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Eigen
        {
            //! Sort eigenvalues d[1..n] and COLUMN eigenvectors v[1..n][1..n]
            /**
             \param d eigenvalues
             \param v eigenvectors, colums
             \param compare comparison for eigenvalues
             */
            template <typename T, typename U, typename COMPARE> inline
            void Sort(Writable<T> &d, Matrix<U> &v, COMPARE &compare)
            {
                assert(v.rows==v.cols);
                assert(v.rows==d.size());
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
                    }
                }
            }


        }


    }
}

#endif

