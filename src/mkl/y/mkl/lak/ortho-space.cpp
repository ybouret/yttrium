
#include "y/mkl/lak/ortho-space.hpp"

namespace Yttrium
{
    namespace MKL
    {
        bool OrthoSpace:: Build(const Matrix<apz> &P)
        {
            const size_t p = P.rows;
            Matrix<apz>  Pt(TransposeOf,P);
            Matrix<apz>  P2(p,p);
            P.mmul(P2,TransposeOf,P);
            std::cerr << "P2=" << P2 << std::endl;
            return false;
        }
    }
}


