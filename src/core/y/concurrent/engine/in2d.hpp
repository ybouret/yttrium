

//! \file

#ifndef Y_Concurrent_Engine2D_Included
#define Y_Concurrent_Engine2D_Included 1

#include "y/concurrent/engine/interface.hpp"
#include "y/concurrent/split/tiling.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        template <typename T>
        class Engine2D : public Engine< AutoPtr<typename Tiling<T>::Tile> >
        {
        public:
            typedef typename Tiling<T>::Tile   Tile;
            typedef AutoPtr<Tile>              Mapping;
            typedef Engine<Mapping>            EngineType;
            typedef typename Tiling<T>::Vertex Vertex;


            inline void start(const ThreadContext &cntx, Vertex lower, Vertex upper)
            {
                Mapping temp = Tiling<T>::Tiles::For(cntx,lower,upper);
                this->initiate(cntx,temp);
            }

            inline virtual ~Engine2D() noexcept {}
            
        protected:
            inline explicit Engine2D() noexcept : EngineType()  {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine2D);
        };
    }

}

#endif

