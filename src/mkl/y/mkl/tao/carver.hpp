//! \file

#ifndef Y_MKL_TAO_Carver_Included
#define Y_MKL_TAO_Carver_Included 1

#include "y/concurrent/loop/simd.hpp"
#include "y/mkl/tao/par/engine1d.hpp"
#include "y/mkl/tao/par/engine2d.hpp"
#include "y/container/matrix/metrics.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            //__________________________________________________________________
            //
            //
            //
            //! handle engines for containers
            //
            //
            //__________________________________________________________________
            class Carver
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Concurrent::SIMD<Engine1D> In1D;   //!< alias
                typedef Concurrent::SIMD<Engine2D> In2D;   //!< alias
                typedef In2D::Vertex               Vertex; //!< alias
                //typedef In1D::Processors            Proc1D; //!< alias
                //typedef In2D::Processors            Proc2D; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Carver(const Concurrent::SharedLoop &); //!< setup engines
                virtual ~Carver() noexcept;                      //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void setup(const size_t         n); //!< setup in1d(1,n,1)
                void setup(const MatrixMetrics &M); //!< setup in2d( (1,1) -> (cols,rows)

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                In1D in1d; //!< 1D engines
                In2D in2d; //!< 2D engines

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Carver);
                const size_t one;
                const Vertex org;
            };
        }

    }
}


#endif

