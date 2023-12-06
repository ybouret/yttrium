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
            typedef typename ENGINE::Locus Locus;

            inline explicit SIMT1D(const SharedLoop &loop) : Processors(loop) {}
            inline virtual ~SIMT1D() noexcept {}

            inline bool matches(const Type head, const Type tail, const Type step) const noexcept
            {
                const Locus &lhs = *(this->locus);
                const Locus  rhs(head,tail,step);
                return lhs == rhs;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT1D);
        };

    }
}

#endif

