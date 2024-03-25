
//! \file

#ifndef Y_CONCURRENT_FRAME2D_INCLUDED
#define Y_CONCURRENT_FRAME2D_INCLUDED 1

#include "y/concurrent/split/tiling.hpp"
#include "y/concurrent/frame/interface.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        template <typename T>
        class Frame2D : public Frame< AutoPtr< typename Tiling<T>::Tile > >
        {
        public:
            typedef T                        Type;
            typedef typename Tiling<T>::Tile Tile;
            typedef AutoPtr<Tile>            Mapping;
            typedef Frame<Mapping>           FrameType;

            inline virtual ~Frame2D() noexcept {}

            inline void init(const V2D<T> &lower, const V2D<T> &upper)
            {
                const Mapping mapping = Tiling<T>::Tiles::For(*this,lower,upper);
                this->workspace.build(mapping);
            }


        protected:
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
