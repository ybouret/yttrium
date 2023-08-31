#include "y/apex/ortho/space.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace Apex
    {

        namespace Ortho
        {

            void Space:: Build(const Matrix<apz> &P)
            {
                const size_t p = P.rows;
                //                const size_t d = P.cols;
                Matrix<apz>  Pt(TransposeOf,P);
                Matrix<apz>  P2(p,p);
                P.mmul(P2,TransposeOf,P);
            }


        }
    }
}

