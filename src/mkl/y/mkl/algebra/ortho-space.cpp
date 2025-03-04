
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/container/cxx/array.hpp"
#include "y/mkl/tao/seq/level3.hpp"

namespace Yttrium
{
    namespace MKL
    {
        bool OrthoSpace:: Build(Matrix<apz>       &Q,
                                const Matrix<apz> &P)
        {
            const size_t p = P.rows; // number of vectors
            const size_t d = P.cols; // number of colums

            Matrix<apz> P2(p,p);
            for(size_t i=p;i>0;--i)
            {
                const Readable<apz> &P_i = P[i];
                for(size_t j=p;j>=p;++j)
                {
                    const Readable<apz> &P_j = P[j];
                    apz                  sum = 0;
                    for(size_t k=d;k>0;--k) sum += P_i[k] * P_j[k];
                    P2[i][j] = P2[j][i] = sum;
                }
            }

            {
                
            }

#if 0
            Antelope::Caddy<apz> xm;
            Matrix<apz>          QQ(d,d);
            {
                apz         dP2 = 0;  // determinant(P*P')
                Matrix<apz> aP2(p,p); // adjoint(P*P')
                Matrix<apz> P2(p,p);  // P*P'
                Tao::MatMul(P2, P, TransposeOf, P, xm);

                // compute det/adj using LU
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


                // finalize QQ
                {
                    const Matrix<apz> Pt(TransposeOf,P);
                    {
                        Matrix<apz> P3(p,d);
                        //aP2.mmul(P3,P);
                        //Pt.mmul(QQ,P3);
                        Tao::MatMul(P3,aP2,P,xm);
                        Tao::MatMul(QQ,Pt,P3,xm);
                    }
                }

                {
                    apz *q = &QQ[1][1];
                    for(size_t i=QQ.items;i>0;--i,++q)
                    {
                        Sign::ReplaceByOpposite( Coerce(q->s) );
                    }
                }

                for(size_t i=d;i>0;--i) QQ[i][i] += dP2;
            }

            // make univocal
            //Kemp::Univocal::MakeMatrix(QQ);

            // avoid trivial repetitions
            //Kemp::Narrow::Down(Q,QQ);
            

            return true;
#endif

        }
    }
}


