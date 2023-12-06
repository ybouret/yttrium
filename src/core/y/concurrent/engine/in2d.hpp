

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
        //______________________________________________________________________
        //
        //
        //
        //! 2D Engine: based of AutoPtr<Tile>
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Engine2D : public Engine< AutoPtr<typename Tiling<T>::Tile> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename Tiling<T>::Tile     Tile;       //!< alias
            typedef typename Tiling<T>::Segment  Segment;    //!< alias
            typedef typename Tiling<T>::Iterator Iterator;   //!< alias
            typedef AutoPtr<Tile>                Mapping;    //!< alias
            typedef Engine<Mapping>              EngineType; //!< alias
            typedef typename Tiling<T>::Vertex   Vertex;     //!< alias
            typedef T                            Type;       //!< alias
            typedef typename Tiling<T>::Locus    Locus;


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
        
            //! start engine: compute tile then initiate/activate
            inline void start(const ThreadContext &cntx, Vertex lower, Vertex upper)
            {
                Mapping temp = Tiling<T>::Tiles::For(cntx,lower,upper);
                this->initiate(cntx,temp);
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline virtual ~Engine2D() noexcept {}
            
        protected:
            inline explicit Engine2D() noexcept : EngineType()  {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine2D);
        };
    }

}

#endif

