
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/apex/mylar.hpp"
#include "y/container/cxx/array.hpp"
#include "y/mkl/tao/seq/level3.hpp"

namespace Yttrium
{
    namespace MKL
    {
        bool OrthoSpace:: Build(Matrix<apz>       &Q,
                                const Matrix<apz> &P)
        {
            const size_t p = P.rows;
            const size_t d = P.cols;

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
            Apex::Mylar::MakeUnivocal(QQ);

            // avoid trivial repetitions
            Apex::Mylar::Compress(Q,QQ);


            return true;
        }
    }
}


