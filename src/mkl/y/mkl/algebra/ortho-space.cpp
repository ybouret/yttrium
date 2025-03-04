
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/mkl/algebra/lu.hpp"

namespace Yttrium
{
    namespace MKL
    {
        bool OrthoSpace:: Build(Matrix<apz>       &Q,
                                const Matrix<apz> &P)
        {
            const size_t p = P.rows; // number of vectors
            const size_t d = P.cols; // number of colums
            Matrix<apz>  P2(p,p);    // P*P'
            apz          dP2 = 0;    // det(P*P')
            Matrix<apz>  aP2(p,p);   // adjoint(P*P')

            for(size_t i=p;i>0;--i)
            {
                const Readable<apz> &P_i = P[i];
                for(size_t j=p;j>=i;--j)
                {
                    const Readable<apz> &P_j = P[j];
                    apz                  sum = 0;
                    for(size_t k=d;k>0;--k) sum += P_i[k] * P_j[k];
                    P2[i][j] = P2[j][i] = sum;
                }
            }



            {
                LU<apq>           lu(p);
                const Matrix<apq> P2_(CopyOf,P2);
                {
                    Matrix<apq>       P2__(P2_);
                    if(!lu.build(P2__)) return false;
                    dP2 = lu.determinant(P2__).numer;
                }
                assert(0!=dP2);
                Matrix<apq> aP2_(p,p);
                lu.adjoint(aP2_,P2_);
                aP2.make(aP2_);
            }



            Matrix<apz> P3(p,d); // aP2 * P
            for(size_t i=p;i>0;--i)
            {
                const Readable<apz> &aP2_i = aP2[i];
                for(size_t j=d;j>0;--j)
                {
                    apz sum =0 ;
                    for(size_t k=p;k>0;--k) sum += aP2_i[k] * P[k][j];
                    P3[i][j] = sum;
                }
            }



            Q.make(d,d);
            for(size_t i=d;i>0;--i)
            {
                for(size_t j=d;j>0;--j)
                {
                    apz sum = 0;
                    for(size_t k=p;k>0;--k) sum += P[k][i] * P3[k][j];
                    Q[i][j] = -sum;
                }
            }

            for(size_t i=d;i>0;--i)
                Q[i][i] += dP2;;


            return true;


        }
    }
}


