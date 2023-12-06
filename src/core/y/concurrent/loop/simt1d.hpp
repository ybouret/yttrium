//! \file

#ifndef Y_Concurrent_SIMT1D_Included
#define Y_Concurrent_SIMT1D_Included 1

#include "y/concurrent/loop/simd.hpp"
#include "y/concurrent/engine/in1d.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //! assuming Engine derives from Engine1D<Type>
        template <class ENGINE>
        class SIMT1D : public SIMD<ENGINE>
        {
        public:
            typedef ENGINE                 EngineType;
            typedef SIMD<ENGINE>           Processors;
            typedef typename ENGINE::Type  Type;

            inline explicit SIMT1D(const SharedLoop &loop) : Processors(loop) {}
            inline virtual ~SIMT1D() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT1D);
        };

    }
}

#endif

