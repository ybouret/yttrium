#include "y/mkl/tao/par/driver.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            Driver:: Driver(const Concurrent::SharedLoop &sl) :
            in1D(sl),
            in2D(sl),
            one(1),
            org(1,1)
            {
            }


            void Driver:: setup(const size_t n)
            {
                in1D.assign(one,n,one);
            }

            void Driver:: setup(const MatrixMetrics &M)
            {
                const Vertex vtx(M.cols,M.rows);
                assert(vtx.x>=1);
                assert(vtx.y>=1);
                in2D.assign(org,vtx);
            }

        }

    }

}

