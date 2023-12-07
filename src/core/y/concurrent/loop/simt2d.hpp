
//! \file

#ifndef Y_Concurrent_SIMT2D_Included
#define Y_Concurrent_SIMT2D_Included 1

#include "y/concurrent/loop/simd.hpp"
#include "y/concurrent/engine/in2d.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! assuming ENGINE derives from Engine2D<Type>
        //
        //
        //______________________________________________________________________
        template <class ENGINE>
        class SIMT2D : public SIMD<ENGINE>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ENGINE                  EngineType; //!< alias to individual engine
            typedef SIMD<ENGINE>            Processors; //!< alias to base class
            typedef typename ENGINE::Type   Type;       //!< alias
            typedef typename ENGINE::Size   Size;       //!< alias
            typedef typename ENGINE::Locus  Locus;      //!< alias
            typedef typename ENGINE::Vertex Vertex;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit SIMT2D(const SharedLoop &loop) : Processors(loop) {}
            inline virtual ~SIMT2D() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! check that request matches current locus
            inline bool matches(const Vertex lower, const Vertex upper) const noexcept
            {
                if( this->locus.isValid() )
                {
                    const Locus &lhs = *(this->locus);
                    const Locus  rhs(lower,upper);
                    return lhs == rhs;
                }
                else
                    return false;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT2D);
        };

    }
}

#endif

