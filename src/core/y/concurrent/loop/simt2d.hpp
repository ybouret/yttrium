
//! \file

#ifndef Y_Concurrent_SIMT2D_Included
#define Y_Concurrent_SIMT2D_Included 1

#include "y/concurrent/loop/simd.hpp"
#include "y/concurrent/engine/in2d.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //! assuming Engine derives from Engine2D<Type>
        template <class ENGINE>
        class SIMT2D : public SIMD<ENGINE>
        {
        public:
            typedef ENGINE                  EngineType;
            typedef SIMD<ENGINE>            Processors;
            typedef typename ENGINE::Type   Type;
            typedef typename ENGINE::Locus  Locus;
            typedef typename ENGINE::Vertex Vertex;

            inline explicit SIMT2D(const SharedLoop &loop) : Processors(loop) {}
            inline virtual ~SIMT2D() noexcept {}

            inline bool matches(const Vertex lower, const Vertex upper) const noexcept
            {
                const Locus &lhs = *(this->locus);
                const Locus  rhs(lower,upper);
                return lhs == rhs;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT2D);
        };

    }
}

#endif

