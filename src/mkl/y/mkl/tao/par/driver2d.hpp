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

            

            //! context for 1D operations
            class Driver2D : public Concurrent::Frame2D<size_t>, public XAddPtr
            {
            public:
                explicit Driver2D(const ThreadContext &) noexcept;
                virtual ~Driver2D() noexcept;

                const Strip * const & strip;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Driver2D);
            };

        }

    }

}

#endif


