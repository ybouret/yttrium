
#include "y/mkl/tao/carver.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            Carver:: ~Carver() noexcept {}

            Carver:: Carver(const Concurrent::SharedLoop &loop) :
            in1d(loop),
            in2d(loop),
            one(1),
            org(1,1)
            {
            }
            
            void Carver:: setup(const size_t n)
            {
                if(!in1d.matches(1,n,1))
                {
                    in1d.dispatch(1,n,1);
                }
            }

            void Carver:: setup(const MatrixMetrics &M)
            {
                const Vertex vtx(M.cols,M.rows);
                if(!in2d.matches(org,vtx))
                {
                    in2d.dispatch(org,vtx);
                }
            }



        }

    }

}

