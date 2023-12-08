
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
            org(one,one)
            {
            }
            
            void Carver:: setup(const size_t n)
            {
                if(n<=0)
                {
                    in1d.quit();
                }
                else
                {
                    in1d.init(one,n,one);
                }
            }

            void Carver:: setup(const MatrixMetrics &M)
            {
                const Vertex vtx(M.cols,M.rows);
                if(vtx == org)
                    in2d.quit();
                else
                {
                    assert(vtx.x>=1);
                    assert(vtx.y>=1);
                    in2d.init(org,vtx);
                }

            }



        }

    }

}

