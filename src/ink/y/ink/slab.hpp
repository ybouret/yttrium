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
            static const size_t Bytes    = 256;              //!< number of Bytes for information
            static const size_t WordSize = sizeof(uint64_t); //!< map on uint64_t[Bytes]
            static const size_t Request  = Bytes * WordSize; //!< memory request

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
            // Methods
            //
            //__________________________________________________________________
            void honorRequest(); //!< once call, post init

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            const HSegment * const & hseg; //!< transmogrified segments [1..count()]




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slab);
            void * const wksp;
            const size_t wlen;
        };

    }

}

#endif

