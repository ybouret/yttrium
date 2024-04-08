
//! \file

#ifndef Y_CONCURRENT_FRAME2D_INCLUDED
#define Y_CONCURRENT_FRAME2D_INCLUDED 1

#include "y/concurrent/split/tiling.hpp"
#include "y/concurrent/frame/interface.hpp"
#include "y/ptr/bare.hpp"

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
            typedef T                           Type;       //!< alias
            typedef typename Tiling<T>::Tile    Tile;       //!< alias
            typedef typename Tiling<T>::Segment Segment;    //!< alias
            typedef AutoPtr<Tile>               Mapping;    //!< alias
            typedef Frame<Mapping>              FrameType;  //!< alias
            typedef BarePtr<const Tile>         TilePtr;    //!< alias
            typedef V2D<T>                      Vertex;     //!< alias

            class Signature
            {
            public:
                inline  Signature(const Vertex &lo, const Vertex &up) noexcept : lower(lo), upper(up) {}
                inline  Signature(const Signature &s) noexcept : lower(s.lower), upper(s.upper) {}
                inline ~Signature() noexcept {}

                inline friend bool operator==(const Signature &lhs, const Signature &rhs) noexcept
                {
                    return (lhs.lower==rhs.lower) && (lhs.upper==rhs.upper);
                }

                inline friend bool operator!=(const Signature &lhs, const Signature &rhs) noexcept
                {
                    return (lhs.lower!=rhs.lower) || (lhs.upper!=rhs.upper);
                }

                const Vertex lower;
                const Vertex upper;
            private:
                Y_DISABLE_ASSIGN(Signature);
            };

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
            inline void assign(const Vertex &lower, const Vertex &upper)
            {
                const Mapping & here = divided(lower,upper);
                if( here.isValid() )
                {
                    Coerce(tile)    = & *here;
                    Coerce(segment) = tile->cxx();
                }
            }

            //! access AutoPtr<Tile> if needed
            inline const Mapping &operator*() const noexcept
            {
                assert(this->workspace.isValid());
                return *(this->workspace);
            }
            
            //! helper to get current count of segments
            inline size_t count() const noexcept
            {
                return 0!=tile ? tile->size : 0;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            const Tile    * const tile;    //!< ACTIVE tile, may be null
            const Segment * const segment; //!< NULL or segment[1..tile->size]

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
            FrameType(ctx), tile(0), segment(0)
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame2D);
            inline virtual void shutdown() noexcept 
            {
                Coerce(tile)     = 0;
                Coerce(segment)  = 0;
            }

            inline const Mapping & divided(const V2D<T> &lower, const V2D<T> &upper)
            {
                assert(0==tile);
                assert(0==segment);
                const Mapping   part = Tiling<T>::Tiles::For(*this,lower,upper);
                return  this->workspace.build(part);
            }

        };

    }

}

#endif
