
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
            /**
             - assign AutoPtr
             - if tile is not NULL, update it
             */
            inline void assign(const V2D<T> &lower, const V2D<T> &upper)
            {
                assert(0==tile);
                const Mapping   part = Tiling<T>::Tiles::For(*this,lower,upper);
                const Mapping & here = this->workspace.build(part);
                if( here.isValid() )
                    Coerce(tile) = & *here;
            }

            //! access AutoPtr<Tile>
            inline const Mapping &operator*() const noexcept
            {
                assert(this->workspace.isValid());
                return *(this->workspace);
            }
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            const Tile * const tile; //!< ACTIVE tile, may be null


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
            FrameType(ctx),
            tile(0)
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame2D);
            inline virtual void shutdown() noexcept { Coerce(tile) = 0; }
        };

    }

}

#endif
