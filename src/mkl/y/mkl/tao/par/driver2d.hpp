//! \file

#ifndef Y_MKL_TAO_Driver2D_Included
#define Y_MKL_TAO_Driver2D_Included 1

#include "y/concurrent/frame/2d.hpp"
#include "y/mkl/tao/par/xaddptr.hpp"
#include "y/mkl/tao/par/strip.hpp"

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
            //! context for 1D operations
            //
            //
            //__________________________________________________________________
            class Driver2D : public Concurrent::Frame2D<size_t>, public XAddPtr
            {
            public:
                explicit Driver2D(const ThreadContext &) noexcept; //!< setup
                virtual ~Driver2D() noexcept;                      //!< cleanup

                const Strip * const & strip; //!< transmogrified segments [1..tile->size] if not NULL

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Driver2D);
            };

        }

    }

}

#endif


