//! \file

#ifndef Y_Concurrent_SIMT1D_Included
#define Y_Concurrent_SIMT1D_Included 1

#include "y/concurrent/loop/simd.hpp"
#include "y/concurrent/engine/in1d.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! assuming ENGINE derives from Engine1D<Type>
        //
        //
        //______________________________________________________________________
        template <class ENGINE>
        class SIMT1D : public SIMD<ENGINE>
        {
        public: 
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ENGINE                 EngineType; //!< alias to individual engine
            typedef SIMD<ENGINE>           Processors; //!< alias to base class
            typedef typename ENGINE::Type  Type;       //!< alias
            typedef typename ENGINE::Size  Size;       //!< alias
            typedef typename ENGINE::Locus Locus;      //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            inline explicit SIMT1D(const SharedLoop &loop) : Processors(loop) {}

            //! cleanup
            inline virtual ~SIMT1D() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
           
            //! check request match current locus=
            inline bool matches(const Type head, const Type tail, const Type step) const noexcept
            {
                if( this->locus.isValid() )
                {
                    const Locus &lhs = *(this->locus);
                    const Locus  rhs(head,tail,step);
                    return lhs == rhs;
                }
                else
                    return false;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT1D);
        };

    }
}

#endif

