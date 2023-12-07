
//! \file

#ifndef Y_MKL_TAO_Motor1D_Included
#define Y_MKL_TAO_Motor1D_Included 1

#include "y/concurrent/engine/in1d.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {
            class Motor1D : public Concurrent::Engine1D<size_t>
            {
            public:
                explicit Motor1D() noexcept;
                virtual ~Motor1D() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Motor1D);
            };
        }
    }
}

#endif
