//! \file

#ifndef Y_MKL_TAO_Driver1D_Included
#define Y_MKL_TAO_Driver1D_Included 1

#include "y/concurrent/frame/1d.hpp"
#include "y/mkl/tao/par/xaddptr.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {


            //__________________________________________________________________
            //
            //
            //! context for 1D operations
            //
            //__________________________________________________________________
            class Driver1D : public Concurrent::Frame1D<size_t>, public XAddPtr
            {
            public:
                explicit Driver1D(const ThreadContext &) noexcept; //!< setup
                virtual ~Driver1D() noexcept;                      //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Driver1D);
            };

            //__________________________________________________________________
            //
            //! alias
            //__________________________________________________________________
            typedef Driver1D::Mapping Mapping1D;

        }

    }

}

#endif

