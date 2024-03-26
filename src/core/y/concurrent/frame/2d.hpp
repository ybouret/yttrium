
//! \file

#ifndef Y_CONCURRENT_FRAME2D_INCLUDED
#define Y_CONCURRENT_FRAME2D_INCLUDED 1

#include "y/concurrent/split/tiling.hpp"
#include "y/concurrent/frame/interface.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Frame for 2D operations
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Frame2D : public Frame< AutoPtr< typename Tiling<T>::Tile > >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef T                        Type;       //!< alias
            typedef typename Tiling<T>::Tile Tile;       //!< alias
            typedef AutoPtr<Tile>            Mapping;    //!< alias
            typedef Frame<Mapping>           FrameType;  //!< alias


            //__________________________________________________________________
            //
            //
            // Method
            //
            //__________________________________________________________________

            //! create sub Tile (may be null)
            inline void attach(const V2D<T> &lower, const V2D<T> &upper)
            {
                const Mapping mapping = Tiling<T>::Tiles::For(*this,lower,upper);
                this->workspace.build(mapping);
            }

            //! access sub Tile, may be NULL
            inline const Tile * operator*() const noexcept
            {
                assert(this->workspace.isValid());
                const Mapping &mapping = *(this->workspace);
                return mapping.isValid() ? & *mapping : 0;
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            inline virtual ~Frame2D() noexcept {}

        protected:
            //! setup
            inline explicit Frame2D(const ThreadContext &ctx) noexcept :
            FrameType(ctx)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame2D);
        };

    }

}

#endif
