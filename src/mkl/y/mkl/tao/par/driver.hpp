
//! \file

#ifndef Y_MKL_TAO_Par_Driver_Included
#define Y_MKL_TAO_Par_Driver_Included 1

#include "y/concurrent/loop/simt.hpp"
#include "y/mkl/tao/par/driver1d.hpp"
#include "y/mkl/tao/par/driver2d.hpp"
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
            class Driver
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Concurrent::SIMT<Driver1D> In1D;     //!< alias
                typedef Concurrent::SIMT<Driver2D> In2D;     //!< alias
                typedef In2D::Vertex               Vertex;   //!< alias
                typedef In1D::AutoUnlink           Unlink1D; //!< alias
                typedef In2D::AutoUnlink           Unlink2D; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Driver(const Concurrent::SharedLoop &); //!< setup engines
                virtual ~Driver() noexcept;                      //!< cleanup

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
                In1D in1D; //!< 1D engines
                In2D in2D; //!< 2D engines

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Driver);
                const size_t one;
                const Vertex org;
            };
        }

    }
}


#endif

