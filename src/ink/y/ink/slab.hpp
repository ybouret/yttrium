//! \file

#ifndef Y_Ink_Slab_Included
#define Y_Ink_Slab_Included 1

#include "y/ink/area.hpp"
#include "y/concurrent/frame/2d.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! HSegment matching Tiling<unit_t>::Segment
        //
        //
        //______________________________________________________________________
        struct HSegment
        {
            unit_t x;  //!< x
            unit_t y;  //!< y
            unit_t w;  //!< width
            unit_t xt; //!< x top
        };

        //______________________________________________________________________
        //
        //
        //
        //! Slab of area to work with threads
        //
        //
        //______________________________________________________________________
        class Slab : public Concurrent::Frame2D<unit_t>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Slab(const ThreadContext &ctx) noexcept; //!< setup
            virtual ~Slab() noexcept;                         //!< cleanup

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            const HSegment * const & hseg; //!< transmogrified segments [1..count()]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slab);
        };

    }

}

#endif

