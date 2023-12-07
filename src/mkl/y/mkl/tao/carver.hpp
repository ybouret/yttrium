//! \file

#ifndef Y_MKL_TAO_Carver_Included
#define Y_MKL_TAO_Carver_Included 1

#include "y/concurrent/loop/simt1d.hpp"
#include "y/concurrent/loop/simt2d.hpp"
#include "y/mkl/tao/par/motor1d.hpp"
#include "y/mkl/tao/par/motor2d.hpp"
#include "y/container/matrix/metrics.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {
            class Carver
            {
            public:
                typedef Concurrent::SIMT1D<Motor1D> In1D;
                typedef Concurrent::SIMT2D<Motor2D> In2D;
                typedef In2D::Vertex                Vertex;

                explicit Carver(const Concurrent::SharedLoop &);
                virtual ~Carver() noexcept;

                void setup(const size_t         n);
                void setup(const MatrixMetrics &M);

                In1D in1d;
                In2D in2d;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Carver);
                const size_t one;
                const Vertex org;
            };
        }

    }
}


#endif

