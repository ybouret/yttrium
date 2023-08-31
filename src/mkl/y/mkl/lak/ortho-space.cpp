
#include "y/mkl/lak/ortho-space.hpp"
#include "y/mkl/lak/lu.hpp"
#include "y/apex/mylar.hpp"
#include "y/container/cxx-array.hpp"

namespace Yttrium
{
    namespace MKL
    {
        bool OrthoSpace:: Build(Matrix<apz> &Q, const Matrix<apz> &P)
        {
            const size_t p = P.rows;
            const size_t d = P.cols;
            assert(Q.rows==d);
            assert(Q.cols==d);

            apz         dP2 = 0;
            {
                Matrix<apz> aP2(p,p);
                {
                    // compute in rational
                    Matrix<apq> P2(p,p);
                    P.mmul(P2,TransposeOf,P);
                    LU<apq> lu(p);

                    // retrieve determinant
                    dP2 = lu.determinant(P2).numer;
                    if(dP2==0)
                        return false;

                    Matrix<apq> aP2_(p,p);
                    lu.adjoint(aP2_,P2);

                    // retrieve adjoint
                    for(size_t i=p;i>0;--i)
                    {
                        for(size_t j=p;j>0;--j)
                        {
                            aP2[i][j] = aP2_[i][j].numer;
                        }
                    }
                }

                {
                    Matrix<apz> aP2P(p,d);
                    aP2.mmul(aP2P,P);
                    Matrix<apz>  Pt(TransposeOf,P);
                    Pt.mmul(Q,aP2P);
                }
            }

            // finalize Q
            for(size_t i=d;i>0;--i)
            {
                for(size_t j=d;j>0;--j)
                {
                    Sign::ReplaceByOpposite( Coerce(Q[i][j].s) );
                }
            }
            for(size_t i=d;i>0;--i) Q[i][i] += dP2;

            // univocal Q
            CxxArray<apq,Memory::Dyadic> q(d);
            for(size_t i=d;i>0;--i)
            {
                for(size_t j=d;j>0;--j) q[j] = Q[i][j];
                Apex::Mylar::Univocal(q);
                for(size_t j=d;j>0;--j) Q[i][j] = q[j].numer;

            }

            return true;
        }
    }
}


